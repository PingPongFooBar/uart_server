#include "wk2114tty.h"
#include <pthread.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
/* 文件描述符全局变量 */
/* 串口芯片控制串口1 */
int ttyAMA1_fd;
/* 串口芯片控制串口2 */
int ttyAMA2_fd;
/* 串口芯片中断文件 */
int irq_fd;
/* 串口访问锁，保证一次读写，写写串口具有原子性 */
pthread_mutex_t uart_mutex1;
pthread_mutex_t uart_mutex2;
/* 串口扩展芯片初始化 */
extern int uart_init();
/* 串口扩展芯片寄存器读取 */
extern void reg_write(char cmd, char data,int fd);
/* 写寄存器 */
extern char reg_read(char cmd,int fd);
/* 通道寄存器读取 */
extern char chn_reg_read(int chn,char cmd,int fd);
/* 通道寄存器烧写 */
extern void chn_reg_write(int chn,char cmd, char data,int fd);
/* 通道fifo 读取 */
extern void fifo_read(char chn,char num,char* rx_data,int fd);
/* 通道fifo 发送 */
extern void fifo_write(char chn,char num,char* tx_data,int fd);
/* 打开某一个串口通道 */
extern int chn_open(int baudrate,int chn);
/* 设置某一个串口通道的波特率 */
extern int set_baudrate(int baudrate,int chn);
/* 关闭某一个串口通道 */
extern int chn_close(int chn);
