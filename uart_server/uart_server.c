/* ���ڷ����������� */
/* ��ɴ����������� */
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
/* 8 ��ͨ�����ź����ݻ����� */
/* ÿһ����������8 ���ֽ� */
char rx_buffers[8][16];
char rx_buffers_leek[8];
/* �����źŻ����� */
char buff_socket_recieve[8][4096];
int buffer_leek[8];
pthread_mutex_t mutex_recv1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_recv2 = PTHREAD_MUTEX_INITIALIZER;
/* ��������������֪ͨ��Ӧ�����Ͻ��յ������� */
pthread_cond_t cond[8] = 																						\
	{
	  PTHREAD_COND_INITIALIZER,PTHREAD_COND_INITIALIZER,PTHREAD_COND_INITIALIZER,PTHREAD_COND_INITIALIZER, \
	  PTHREAD_COND_INITIALIZER,PTHREAD_COND_INITIALIZER,PTHREAD_COND_INITIALIZER,PTHREAD_COND_INITIALIZER  \
	 };
/* ����������Ӧÿһ���˿ڸ���һ�� */
pthread_mutex_t mutex[8] = 																						\
	{
	PTHREAD_MUTEX_INITIALIZER,PTHREAD_MUTEX_INITIALIZER,PTHREAD_MUTEX_INITIALIZER,PTHREAD_MUTEX_INITIALIZER,	\
	PTHREAD_MUTEX_INITIALIZER,PTHREAD_MUTEX_INITIALIZER,PTHREAD_MUTEX_INITIALIZER,PTHREAD_MUTEX_INITIALIZER	\
	};
/* ����ֵ����ֹ��ٻ��� */
int condition[8];
/* ͨ��ʹ�ü�¼���以���� */
int chn_used[8];
pthread_mutex_t mutex_used = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv)
{
	int i,sock_fd,cli_fd;
	pthread_t net_tid,monitor_tid;
	//��ֹ������дһ���ѶϿ�����ʱ��ֱ���˳�����
	signal(SIGPIPE, SIG_IGN);
	//���ڳ�ʼ��
	pthread_mutex_init(&uart_mutex1,NULL);
	pthread_mutex_init(&uart_mutex2,NULL);
	if(uart_init() < 0)
		return -1;
	//��������SOCKET��������
	if(creat_socket(&sock_fd,PORT_UART) == -1){
		printf("Fail to create connection.\n");
		return -1;
	}
	//�������ڶ�д����
	if(pthread_create(&monitor_tid,NULL,irq_monitor_fd,&ttyAMA1_fd))
		printf("Fail to creat monitor thread.\n");
	if(pthread_create(&monitor_tid,NULL,irq_monitor_fd,&ttyAMA2_fd))
		printf("Fail to creat monitor thread.\n");
	//	����ͻ�������
	while(1){
		if((cli_fd = accept(sock_fd,NULL,NULL)) == -1){
			printf("%s\n", strerror(errno));
			continue;
		}else{
			printf("connected!\n");
		}
		//�����ͻ���ͨ���߳�
		//ע�⣺�ͻ����߳�Ҫô��Ҫjoin Ҫô����Ϊ����̬����������ڴ�й©
		if(pthread_create(&net_tid,NULL,sock_recv,(void *)cli_fd))
			printf("Fail to create thread.\n");
	}
	return 0;
}

//���ڷ�������������֮��
//����һ���ַ���ʾҪͨ�ŵĴ��ں�
//�ٷ���һ���ַ���ʾ���ڲ�����
//һ�����Ӷ�Ӧһ�����ں�
//���ӽ����򽫴��ڹر�
void* sock_recv(void *arg){
	int cli_fd = (int)arg;
	int chn,baudrate;
	int nread;
	int fd = ttyAMA1_fd;
	char baudrate_buff[1024];
	pthread_t uart_tid;
	struct thread_arg t_arg;
//�����̷߳���,��ֹ�ڴ�й©
	pthread_detach(pthread_self());
//�Ƚ���һ���ַ�����ʾͨ�Ŵ��ں�
	if(read(cli_fd,&chn,1) == 0){
		close(cli_fd);
		return NULL;
	}
	t_arg.chn = chn;
	t_arg.fd = cli_fd;
//�ٽ���һ���ַ�������ʾ������
	memset(baudrate_buff,0,1024);
	if(read(cli_fd,baudrate_buff,1024) == 0){
		close(cli_fd);
		return NULL;
	}
	baudrate = atoi(baudrate_buff);
	pthread_mutex_lock(&mutex_used);
//�˴�������ͨ���жϣ��Ƿ���Чͨ����ͨ���Ƿ�����ʹ��
	if(chn > 7 || chn < 0 || baudrate < 0 || baudrate > 115200 || chn_used[chn]){
		close(cli_fd);
		printf("para erro or chn is used,chn : % d ,used %d,baudrate : %d\n",chn,chn_used[chn],baudrate);
		pthread_mutex_unlock(&mutex_used);
		return NULL;
	}
//�򿪴���
	chn_open(baudrate, chn);
	chn_used[chn] = 1;
	pthread_mutex_unlock(&mutex_used);
	printf("open chn %d with baudrate %d\n",chn,baudrate);
//�������ڽ����̣߳������ڽ��յ������ݷ��͸���λ��
	if(pthread_create(&uart_tid,NULL,uart_recv,&t_arg))
		printf("Fail to create uart_recv thread.\n");	
	if(chn >= 4){
		chn -= 4;
		fd = ttyAMA2_fd;
	}
//�������緢�������ź�
	while(1){
		/* ���յ��ͻ��˵�FIN �����ź� */
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

//�������ݵĽ���ʹ��������������ģ��
//������ͳһ��ȡ�жϣ��ж��ж�Դ����ȡ���ݣ�֪ͨ�ȴ���Ӧ�ж�Դ��������
//�������������������ź���ʵ�֣��˴�ʹ����������
void* uart_recv(void * arg)
{
	int cli_fd = ((struct thread_arg *)arg)->fd;
	int chn = ((struct thread_arg *)arg)->chn;
	char uart_buff;
	//�߳��ⲿ�˳���Ϊ��ֹ��û���ͷ�, ��Ҫ�ͷ���
	pthread_cleanup_push(clean_func,&mutex[chn]);
	printf("uart_recv for chn : %d \n",chn);
	//�˴�Ϊ�����ߣ���������ݣ�ֱ���þͿ���
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


//��ȡ�ж��źŵ�������
/* ʹ�ö��������������������� */
//ʵ�������������ڿ�ͬʱʹ�ã���ô�ֳ������̶߳�ȡ�����������ʸ���
void* irq_monitor_fd(void * arg)
{
	//char uart_buff;
	int i,chn;
	int data_size;
	int * p = arg;
	int fd = *p;
	while(1){
		//�ж��ж�Դ,��������������ȡ������������֪ͨ�����߳�
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
