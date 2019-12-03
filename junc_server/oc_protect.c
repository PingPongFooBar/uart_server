#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include "junction_app.h"

int fd;
char path[20];
void oc_handler(int num){
	int i;
	ioctl(fd,OFF);
}

/* 参数1 用来表示第几个设备 */
int main(int argc, char **argv)
{
	int oflag;
	snprintf(path,20,"/dev/junction%s",argv[1]);
	fd = open(path,O_RDWR);
	signal(SIGIO,oc_handler);
	fcntl(fd, F_SETOWN, getpid());
	oflag = fcntl(fd, F_GETFL);
	fcntl(fd, F_SETFL, oflag | FASYNC);

	while(1);
	return 0;	
}
