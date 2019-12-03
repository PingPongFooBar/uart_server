/* 接驳控制服务器主函数 */
/* 完成联网接驳控制功能 */
#include "sock.h"
#include "junc.h"
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
#include <sys/ioctl.h>

void* sock_recv(void *arg);
void* io_state(void * arg);

int main(int argc, char **argv)
{
	int i,sock_fd,cli_fd;
	pthread_t net_tid;
	//防止服务器写一个已断开连接时，直接退出程序
	signal(SIGPIPE, SIG_IGN);
	//通道初始化
	if(junc_init() < 0)
		return -1;
	//创建网络SOCKET，并监听
	if(creat_socket(&sock_fd,PORT_JUNC) == -1){
		printf("Fail to create connection.\n");
		return -1;
	}
	//	处理客户端连接
	while(1){
		if((cli_fd = accept(sock_fd,NULL,NULL)) == -1){
			printf("%s\n", strerror(errno));
			continue;
		}else{
			printf("connected!\n");
		}
		//创建客户端通信线程
		if(pthread_create(&net_tid,NULL,sock_recv,(void *)cli_fd))
			printf("Fail to create thread.\n");
	}
	return 0;
}

//接驳控制客户端创建连接之后，
//发送一个字符表示要控制的通道号
//多个连接可以控制同一个通道
void* sock_recv(void *arg){
	int cli_fd = (int)arg;
	char buff_socket_recieve[1024];
	int chn;
	int nread;
	pthread_t junc_tid;
	struct thread_arg t_arg;
//设置线程分离,防止内存泄漏
	pthread_detach(pthread_self());
//先接收一个字符，表示控制通道号
	if(read(cli_fd,&buff_socket_recieve,1024) == 0){
		close(cli_fd);
		return NULL;
	}
	chn = buff_socket_recieve[0];
	t_arg.chn = chn;
	t_arg.fd = cli_fd;
//创建状态监视线程，将通道状态信息发送给上位机
	if(pthread_create(&junc_tid,NULL,io_state,&t_arg))
		printf("Fail to create uart_recv thread.\n");	
//处理网络发过来的信号
	while(1){
		/* 接收到客户端的FIN 结束信号 */
		if((nread = read(cli_fd,&buff_socket_recieve,1)) == 0){
			break;
		}else if(nread < 0){
			if(errno == EINTR)
				continue;
			else
				break;
		}
		//处理指令
		switch(buff_socket_recieve[0]){
		case 0:
			ioctl(juns[chn],ON);
			break;
		case 1:
			ioctl(juns[chn],OFF);
			break;
		case 2:
			ioctl(juns[chn],OC_RST);
			break;
		default:
			break;
		}
	}
	pthread_cancel(junc_tid);
	pthread_join(junc_tid,NULL);
	close(cli_fd);
}

/* io状态发送程序 */
void* io_state(void * arg)
{
	int cli_fd = ((struct thread_arg *)arg)->fd;
	int chn = ((struct thread_arg *)arg)->chn;
	struct io_val_t io_val;
	char state;
	int n;
	//每隔一秒读取通道状态，并发送给上位机
	while(1){
		n = read(juns[chn],&io_val,sizeof(struct io_val_t));
		state = io_val.K1 | io_val.K2 << 1 | io_val.OC << 2 | io_val.RST << 3;
		write(cli_fd,&state,1);
		sleep(1);
	}
}
