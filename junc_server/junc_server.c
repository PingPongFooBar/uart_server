/* �Ӳ����Ʒ����������� */
/* ��������Ӳ����ƹ��� */
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
	//��ֹ������дһ���ѶϿ�����ʱ��ֱ���˳�����
	signal(SIGPIPE, SIG_IGN);
	//ͨ����ʼ��
	if(junc_init() < 0)
		return -1;
	//��������SOCKET��������
	if(creat_socket(&sock_fd,PORT_JUNC) == -1){
		printf("Fail to create connection.\n");
		return -1;
	}
	//	����ͻ�������
	while(1){
		if((cli_fd = accept(sock_fd,NULL,NULL)) == -1){
			printf("%s\n", strerror(errno));
			continue;
		}else{
			printf("connected!\n");
		}
		//�����ͻ���ͨ���߳�
		if(pthread_create(&net_tid,NULL,sock_recv,(void *)cli_fd))
			printf("Fail to create thread.\n");
	}
	return 0;
}

//�Ӳ����ƿͻ��˴�������֮��
//����һ���ַ���ʾҪ���Ƶ�ͨ����
//������ӿ��Կ���ͬһ��ͨ��
void* sock_recv(void *arg){
	int cli_fd = (int)arg;
	char buff_socket_recieve[1024];
	int chn;
	int nread;
	pthread_t junc_tid;
	struct thread_arg t_arg;
//�����̷߳���,��ֹ�ڴ�й©
	pthread_detach(pthread_self());
//�Ƚ���һ���ַ�����ʾ����ͨ����
	if(read(cli_fd,&buff_socket_recieve,1024) == 0){
		close(cli_fd);
		return NULL;
	}
	chn = buff_socket_recieve[0];
	t_arg.chn = chn;
	t_arg.fd = cli_fd;
//����״̬�����̣߳���ͨ��״̬��Ϣ���͸���λ��
	if(pthread_create(&junc_tid,NULL,io_state,&t_arg))
		printf("Fail to create uart_recv thread.\n");	
//�������緢�������ź�
	while(1){
		/* ���յ��ͻ��˵�FIN �����ź� */
		if((nread = read(cli_fd,&buff_socket_recieve,1)) == 0){
			break;
		}else if(nread < 0){
			if(errno == EINTR)
				continue;
			else
				break;
		}
		//����ָ��
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

/* io״̬���ͳ��� */
void* io_state(void * arg)
{
	int cli_fd = ((struct thread_arg *)arg)->fd;
	int chn = ((struct thread_arg *)arg)->chn;
	struct io_val_t io_val;
	char state;
	int n;
	//ÿ��һ���ȡͨ��״̬�������͸���λ��
	while(1){
		n = read(juns[chn],&io_val,sizeof(struct io_val_t));
		state = io_val.K1 | io_val.K2 << 1 | io_val.OC << 2 | io_val.RST << 3;
		write(cli_fd,&state,1);
		sleep(1);
	}
}
