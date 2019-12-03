/* 串口服务器主函数 */
/* 完成串口联网功能 */
#include "sock.h"
#include "uart.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

void* sock_recv(void *arg);
void* uart_recv(void * arg);
void* irq_monitor_fd(void * arg);
void clean_func(void * arg);
/* 8 个通道的信号数据缓冲区 */
/* 每一个缓冲区有8 个字节 */
char rx_buffers[8][16];
char rx_buffers_leek[8];
/* 网络信号缓冲区 */
char buff_socket_recieve[8][4096];
int buffer_leek[8];
pthread_mutex_t mutex_recv1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_recv2 = PTHREAD_MUTEX_INITIALIZER;
/* 条件变量，用于通知对应串口上接收到数据了 */
pthread_cond_t cond[8] = 																						\
	{
	  PTHREAD_COND_INITIALIZER,PTHREAD_COND_INITIALIZER,PTHREAD_COND_INITIALIZER,PTHREAD_COND_INITIALIZER, \
	  PTHREAD_COND_INITIALIZER,PTHREAD_COND_INITIALIZER,PTHREAD_COND_INITIALIZER,PTHREAD_COND_INITIALIZER  \
	 };
/* 互斥量，对应每一个端口各有一个 */
pthread_mutex_t mutex[8] = 																						\
	{
	PTHREAD_MUTEX_INITIALIZER,PTHREAD_MUTEX_INITIALIZER,PTHREAD_MUTEX_INITIALIZER,PTHREAD_MUTEX_INITIALIZER,	\
	PTHREAD_MUTEX_INITIALIZER,PTHREAD_MUTEX_INITIALIZER,PTHREAD_MUTEX_INITIALIZER,PTHREAD_MUTEX_INITIALIZER	\
	};
/* 条件值，防止虚假唤醒 */
int condition[8];
/* 通道使用记录及其互斥量 */
int chn_used[8];
pthread_mutex_t mutex_used = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv)
{
	int i,sock_fd,cli_fd;
	pthread_t net_tid,monitor_tid;
	//防止服务器写一个已断开连接时，直接退出程序
	signal(SIGPIPE, SIG_IGN);
	//串口初始化
	pthread_mutex_init(&uart_mutex1,NULL);
	pthread_mutex_init(&uart_mutex2,NULL);
	if(uart_init() < 0)
		return -1;
	//创建网络SOCKET，并监听
	if(creat_socket(&sock_fd,PORT_UART) == -1){
		printf("Fail to create connection.\n");
		return -1;
	}
	//创建串口读写程序
	if(pthread_create(&monitor_tid,NULL,irq_monitor_fd,&ttyAMA1_fd))
		printf("Fail to creat monitor thread.\n");
	if(pthread_create(&monitor_tid,NULL,irq_monitor_fd,&ttyAMA2_fd))
		printf("Fail to creat monitor thread.\n");
	//	处理客户端连接
	while(1){
		if((cli_fd = accept(sock_fd,NULL,NULL)) == -1){
			printf("%s\n", strerror(errno));
			continue;
		}else{
			printf("connected!\n");
		}
		//创建客户端通信线程
		//注意：客户端线程要么需要join 要么设置为分离态，否则出现内存泄漏
		if(pthread_create(&net_tid,NULL,sock_recv,(void *)cli_fd))
			printf("Fail to create thread.\n");
	}
	return 0;
}

//串口服务器创建连接之后，
//发送一个字符表示要通信的串口号
//再发送一个字符表示串口波特率
//一个连接对应一个串口号
//连接结束则将串口关闭
void* sock_recv(void *arg){
	int cli_fd = (int)arg;
	int chn,baudrate;
	int nread;
	int fd = ttyAMA1_fd;
	char baudrate_buff[1024];
	pthread_t uart_tid;
	struct thread_arg t_arg;
//设置线程分离,防止内存泄漏
	pthread_detach(pthread_self());
//先接收一个字符，表示通信串口号
	if(read(cli_fd,&chn,1) == 0){
		close(cli_fd);
		return NULL;
	}
	t_arg.chn = chn;
	t_arg.fd = cli_fd;
//再接收一个字符串，表示波特率
	memset(baudrate_buff,0,1024);
	if(read(cli_fd,baudrate_buff,1024) == 0){
		close(cli_fd);
		return NULL;
	}
	baudrate = atoi(baudrate_buff);
	pthread_mutex_lock(&mutex_used);
//此处最好添加通道判断，是否有效通道，通道是否正在使用
	if(chn > 7 || chn < 0 || baudrate < 0 || baudrate > 115200 || chn_used[chn]){
		close(cli_fd);
		printf("para erro or chn is used,chn : % d ,used %d,baudrate : %d\n",chn,chn_used[chn],baudrate);
		pthread_mutex_unlock(&mutex_used);
		return NULL;
	}
//打开串口
	chn_open(baudrate, chn);
	chn_used[chn] = 1;
	pthread_mutex_unlock(&mutex_used);
	printf("open chn %d with baudrate %d\n",chn,baudrate);
//创建串口接收线程，将串口接收到的数据发送给上位机
	if(pthread_create(&uart_tid,NULL,uart_recv,&t_arg))
		printf("Fail to create uart_recv thread.\n");	
	if(chn >= 4){
		chn -= 4;
		fd = ttyAMA2_fd;
	}
//处理网络发过来的信号
	while(1){
		/* 接收到客户端的FIN 结束信号 */
		if((nread = read(cli_fd,baudrate_buff,1024)) == 0){
			break;
		}else if(nread < 0){
			if(errno == EINTR)
				continue;
			else
				break;
		}
		if(t_arg.chn < 4)
			pthread_mutex_lock(&mutex_recv1);
		else
			pthread_mutex_lock(&mutex_recv2);
		if(nread > 4096 - buffer_leek[t_arg.chn]) nread = 4096 - buffer_leek[t_arg.chn];
		memcpy(buff_socket_recieve[t_arg.chn] + buffer_leek[t_arg.chn], baudrate_buff ,nread);
		buffer_leek[t_arg.chn] += nread;
		if(t_arg.chn < 4)
			pthread_mutex_unlock(&mutex_recv1);
		else
			pthread_mutex_unlock(&mutex_recv2);
	}
	pthread_cancel(uart_tid);
	pthread_join(uart_tid,NULL);
	close(cli_fd);
	chn_close(t_arg.chn);
	buffer_leek[t_arg.chn] = 0;
	pthread_mutex_lock(&mutex_used);
	chn_used[t_arg.chn] = 0;
	pthread_mutex_unlock(&mutex_used);
}

//串口数据的接收使用生产者消费者模型
//生产者统一读取中断，判断中断源，读取数据，通知等待对应中断源的消费者
//可以用条件变量或者信号量实现，此处使用条件变量
void* uart_recv(void * arg)
{
	int cli_fd = ((struct thread_arg *)arg)->fd;
	int chn = ((struct thread_arg *)arg)->chn;
	char uart_buff;
	//线程外部退出，为防止锁没有释放, 需要释放锁
	pthread_cleanup_push(clean_func,&mutex[chn]);
	printf("uart_recv for chn : %d \n",chn);
	//此处为消费者，如果有数据，直接拿就可以
	while(1){
		pthread_mutex_lock(&mutex[chn]);
		while(!condition[chn]){
			pthread_cond_wait(&cond[chn],&mutex[chn]);
		}
		write(cli_fd,rx_buffers[chn],rx_buffers_leek[chn]);
		condition[chn] = 0;
		pthread_mutex_unlock(&mutex[chn]);
	}
	pthread_cleanup_pop(0);
}
void clean_func(void * arg){
	pthread_mutex_unlock((pthread_mutex_t*)arg);
}


//读取中断信号的生产者
/* 使用多个条件变量，多个互斥量 */
//实际上有两个串口可同时使用，那么分成两个线程读取，串口利用率更高
void* irq_monitor_fd(void * arg)
{
	//char uart_buff;
	int i,chn;
	int data_size;
	int * p = arg;
	int fd = *p;
	while(1){
		//判断中断源,如果有新数据则读取到缓冲区，并通知其他线程
		for(i = 0;i < 4;i++){
			chn  = fd == ttyAMA1_fd ? i : i + 4;
			if(chn_reg_read(i,SIFR,fd) & 0x01){
				data_size = chn_reg_read(i,RFCNT,fd);
				if(data_size == 0 || data_size > 16) 
					data_size = 16;
				pthread_mutex_lock(&mutex[chn]);
				fifo_read(i, data_size, rx_buffers[chn] ,fd);
				rx_buffers_leek[chn] = data_size;
				condition[chn] = 1;
				pthread_mutex_unlock(&mutex[chn]);
				pthread_cond_signal(&cond[chn]);
			}
			if(chn == i)
				pthread_mutex_lock(&mutex_recv1);
			else
				pthread_mutex_lock(&mutex_recv2);
			if(buffer_leek[chn] != 0){
				int byte_to_write = buffer_leek[chn];
				if(byte_to_write > 16) byte_to_write = 16;
				buffer_leek[chn] -= byte_to_write;
				fifo_write(i, byte_to_write, buff_socket_recieve[chn] + buffer_leek[chn], fd);
			}
			if(chn == i)
				pthread_mutex_unlock(&mutex_recv1);
			else
				pthread_mutex_unlock(&mutex_recv2);
		}
	}
}
