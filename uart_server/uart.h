#include "wk2114tty.h"
#include <pthread.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
/* �ļ�������ȫ�ֱ��� */
/* ����оƬ���ƴ���1 */
int ttyAMA1_fd;
/* ����оƬ���ƴ���2 */
int ttyAMA2_fd;
/* ����оƬ�ж��ļ� */
int irq_fd;
/* ���ڷ���������֤һ�ζ�д��дд���ھ���ԭ���� */
pthread_mutex_t uart_mutex1;
pthread_mutex_t uart_mutex2;
/* ������չоƬ��ʼ�� */
extern int uart_init();
/* ������չоƬ�Ĵ�����ȡ */
extern void reg_write(char cmd, char data,int fd);
/* д�Ĵ��� */
extern char reg_read(char cmd,int fd);
/* ͨ���Ĵ�����ȡ */
extern char chn_reg_read(int chn,char cmd,int fd);
/* ͨ���Ĵ�����д */
extern void chn_reg_write(int chn,char cmd, char data,int fd);
/* ͨ��fifo ��ȡ */
extern void fifo_read(char chn,char num,char* rx_data,int fd);
/* ͨ��fifo ���� */
extern void fifo_write(char chn,char num,char* tx_data,int fd);
/* ��ĳһ������ͨ�� */
extern int chn_open(int baudrate,int chn);
/* ����ĳһ������ͨ���Ĳ����� */
extern int set_baudrate(int baudrate,int chn);
/* �ر�ĳһ������ͨ�� */
extern int chn_close(int chn);
