#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define	PORT_UART			8564
#define	PORT_JUNC			8560
#define	IP					"10.13.106.12"
#define	MAX_CLIENT_NUM	10
/* ����Ӳ�����Ϸ������ݵ��̲߳��� */
struct thread_arg{
	int fd;
	int chn;
};

extern int creat_socket(int * server_fd, int port);