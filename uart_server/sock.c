/* 网络相关的函数 */
#include "sock.h"

int creat_socket(int * server_fd, int port)
{
	int sock_fd = *server_fd;
	struct sockaddr_in ser_addr;
//创建TCP 套接字
	sock_fd = socket(AF_INET,SOCK_STREAM,0);
	if(sock_fd == -1){
		printf("fail to creat sock_fd\n");
		return -1;
	}else{
		printf("sock_fd created!\n");
	}
	*server_fd = sock_fd;
//设置本地地址
	bzero(&ser_addr,sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(port);
	ser_addr.sin_addr.s_addr = inet_addr(IP);
	bzero(&(ser_addr.sin_zero),8);
//	命名套接字
	if(bind(sock_fd,(struct sockaddr *)&ser_addr,sizeof(ser_addr)) != 0){
		printf("fail to bind addr\n");
		close (sock_fd);
		return -1;
	}else{
		printf("bind %s successfully!\n",IP);
	}
//	开始监听
	if(listen(sock_fd,MAX_CLIENT_NUM) != 0){
		printf("fail to create listener\n");
		close (sock_fd);
		return -1;
	}else{
		printf("listener created!\n");
		printf("waiting for connection\n");
	}
	return 0;
}
