#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include "junction_app.h"

/* 第一个参数为命令，第二个参数为第几路 */
int main(int argc, char **argv)
{
	int fd;
	char path[20];
	snprintf(path,20,"/dev/junction%s",argv[2]);
	fd = open(path,O_RDWR);

	if(!strcmp(argv[1],"on"))
		ioctl(fd,ON);
	
	else if(!strcmp(argv[1],"off"))
		ioctl(fd,OFF);
	else if(!strcmp(argv[1],"rst"))
		
		ioctl(fd,OC_RST);

	close(fd);
	
	return 0;	
}
