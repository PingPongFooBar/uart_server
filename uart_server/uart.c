/* ����оƬ��ص��������� */
#include "uart.h"

int uart_init()
{
	struct termios opt;
	char cmd;
	int i;
	ttyAMA1_fd = open("/dev/ttyAMA1",O_RDWR);
	if(ttyAMA1_fd < 0){
		printf("Fail to open serial1 device.\n");
		return -1;
	}
	ttyAMA2_fd = open("/dev/ttyAMA2",O_RDWR);
	if(ttyAMA1_fd < 0){
		printf("Fail to open serial1 device.\n");
		return -1;
	}
	irq_fd = open("/dev/irq_dev",O_RDWR);
	if(irq_fd < 0){
		printf("Fail to open irq device.\n");
		return -1;
	}
	
	printf("ttyAMA1 and 2 open successful\n");
//��ȡ����
	tcgetattr(ttyAMA1_fd,&opt);
	tcflush(ttyAMA1_fd,TCIOFLUSH);
	tcflush(ttyAMA2_fd,TCIOFLUSH);
//�����ն�����Ϊԭʼģʽ
	cfmakeraw(&opt);
//���ò�����
	cfsetispeed(&opt,B576000);
	cfsetospeed(&opt,B576000);
//8N1
	opt.c_cflag &= ~CSIZE;
	opt.c_cflag |= CS8;
//��У��
	opt.c_cflag &= ~PARENB;
	opt.c_iflag &= ~INPCK;
//ֹͣλ
	opt.c_cflag &= ~CSTOPB;
//1�볬ʱ������ֱ��������һ���ֽڻ��߳�ʱ
	opt.c_cc[VTIME] = 10;
	opt.c_cc[VMIN] = 0;
//��������
	tcsetattr(ttyAMA1_fd,TCSANOW,&opt);
	tcflush(ttyAMA1_fd,TCIOFLUSH);

	tcsetattr(ttyAMA2_fd,TCSANOW,&opt);
	tcflush(ttyAMA2_fd,TCIOFLUSH);

	cmd = 0x55;
	write(ttyAMA1_fd,&cmd,1);
	tcflush(ttyAMA1_fd,TCIOFLUSH);
	write(ttyAMA2_fd,&cmd,1);
	tcflush(ttyAMA2_fd,TCIOFLUSH);
	usleep(1000);
//��ʼ��WK2114
	reg_write(GRST, 0x0f,ttyAMA1_fd);
	printf("reset all uart\n");
	reg_write(GENA, 0xff,ttyAMA1_fd);
//�ж�ʹ��
	reg_write(GIER,0x0f,ttyAMA1_fd);
//��ʼ��WK2114
	reg_write(GRST, 0x0f,ttyAMA2_fd);
	printf("reset all uart\n");
	reg_write(GENA, 0xff,ttyAMA2_fd);
//�ж�ʹ��
	reg_write(GIER,0x0f,ttyAMA2_fd);

	return 0;
}

int chn_open(int baudrate,int chn){
	int fd = ttyAMA1_fd;
	if(set_baudrate(baudrate, chn) < 0) return -1;
	if(chn >= 4) {
		chn -= 4;
		fd = ttyAMA2_fd;
	}
	//ʹ��ͨ��TX,RX��ʹ��ͨ��1FIFO
	chn_reg_write(chn,SPAGE, 0x01,fd);
	chn_reg_write(chn,RFTL,0x01,fd);//���ý��մ���Ϊ1
	chn_reg_write(chn,SPAGE, 0x00,fd);
	chn_reg_write(chn,SCR, 0x03,fd);
	chn_reg_write(chn,FCR,0x0f,fd);
	//�Ӵ����ж�ʹ��,���մ����ж�
	chn_reg_write(chn,SIER, 0x01,fd);
	return 0;
}

int chn_close(int chn)
{
	int fd = ttyAMA1_fd;
	if(chn >= 4) {
		chn -= 4;
		fd = ttyAMA2_fd;
	}
	//�ر�ͨ��TX,RX��ʹ��ͨ��1FIFO
	chn_reg_write(chn,SPAGE, 0x00,fd);
	chn_reg_write(chn,SCR, 0x00,fd);
	chn_reg_write(chn,FCR,0x00,fd);
	//�Ӵ����жϽ�ֹ,���մ����ж�
	chn_reg_write(chn,SIER, 0x00,fd);
	return 0;
}

int set_baudrate(int baudrate,int chn){
	/* �����ʵ�8 λ����8 λ�Լ�С����λ */
	int fd = ttyAMA1_fd;
	unsigned short  baudrate0,baudrate1,pres;
	unsigned int reg;
	/* ���㲨���ʼĴ�����ֵ */
	reg = FS_110592 / (baudrate * 16);
	baudrate0 = (reg & 0xFF) - 1;
	baudrate1 = (reg >> 8) & 0xFF;
	pres = ((FS_110592 * 10) / (baudrate * 16)) & 0x01;
	if(chn > 8 || chn < 0) return -1;
	if(chn >= 4) {
		chn -= 4;
		fd = ttyAMA2_fd;
	}
	//ͨ�����ò�����115200��ʹ�ܷ��ͽ��ա�Ĭ��8N1
	chn_reg_write(chn,SPAGE, 0x01,fd);
	chn_reg_write(chn,BAUD0, baudrate0,fd);
	chn_reg_write(chn,BAUD1, baudrate1,fd);
	chn_reg_write(chn,PRES, pres,fd);
	return 0;
}


void reg_write(char cmd, char data,int fd){
	cmd |= (00<<6);
	
	if(fd == ttyAMA1_fd)
		pthread_mutex_lock(&uart_mutex1);
	else
		pthread_mutex_lock(&uart_mutex2);
	
	write(fd,&cmd,1);
	write(fd,&data,1);

	if(fd == ttyAMA1_fd)
		pthread_mutex_unlock(&uart_mutex1);
	else
		pthread_mutex_unlock(&uart_mutex2);
	
}

char reg_read(char cmd,int fd){
	//Ϊ�˷�ֹ���ڻ�����ţ��˾�Ҫ��
	tcflush(fd,TCIOFLUSH);
	char data;
	cmd |= (01<<6);
	
	if(fd == ttyAMA1_fd)
		pthread_mutex_lock(&uart_mutex1);
	else
		pthread_mutex_lock(&uart_mutex2);
	
	write(fd,&cmd,1);
	read(fd,&data,1);
	
	if(fd == ttyAMA1_fd)
		pthread_mutex_unlock(&uart_mutex1);
	else
		pthread_mutex_unlock(&uart_mutex2);
	
	return data;
}

char chn_reg_read(int chn,char cmd,int fd){
	//Ϊ�˷�ֹ���ڻ�����ţ��˾�Ҫ��
	tcflush(fd,TCIOFLUSH);
	char data;
	cmd |= (chn << 4)|(01<<6);
	
	if(fd == ttyAMA1_fd)
		pthread_mutex_lock(&uart_mutex1);
	else
		pthread_mutex_lock(&uart_mutex2);
	write(fd,&cmd,1);
	read(fd,&data,1);
	if(fd == ttyAMA1_fd)
		pthread_mutex_unlock(&uart_mutex1);
	else
		pthread_mutex_unlock(&uart_mutex2);
	return data;
}

void chn_reg_write(int chn,char cmd, char data,int fd){
	cmd |= (chn << 4)|(00<<6);
	
	if(fd == ttyAMA1_fd)
		pthread_mutex_lock(&uart_mutex1);
	else
		pthread_mutex_lock(&uart_mutex2);
	write(fd,&cmd,1);
	write(fd,&data,1);
	if(fd == ttyAMA1_fd)
		pthread_mutex_unlock(&uart_mutex1);
	else
		pthread_mutex_unlock(&uart_mutex2);
	
}

void fifo_read(char chn,char num,char* rx_data,int fd){
	char* uart_data;
	uart_data = rx_data;
	char cmd = ((11<<6)|(chn<<4)|(num - 1));
	
	if(fd == ttyAMA1_fd)
		pthread_mutex_lock(&uart_mutex1);
	else
		pthread_mutex_lock(&uart_mutex2);
	
	write(fd,&cmd,1);
	read(fd,uart_data,num);
	
	if(fd == ttyAMA1_fd)
		pthread_mutex_unlock(&uart_mutex1);
	else
		pthread_mutex_unlock(&uart_mutex2);
	
}

void fifo_write(char chn,char num,char* tx_data,int fd){
	char* uart_data;
	uart_data = tx_data;
	char cmd = ((10<<6)|(chn<<4)|(num - 1));
	
	if(fd == ttyAMA1_fd)
		pthread_mutex_lock(&uart_mutex1);
	else
		pthread_mutex_lock(&uart_mutex2);
	
	write(fd,&cmd,1);
	write(fd,uart_data,num);
	
	if(fd == ttyAMA1_fd)
		pthread_mutex_unlock(&uart_mutex1);
	else
		pthread_mutex_unlock(&uart_mutex2);
	
}


