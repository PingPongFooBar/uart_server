#include <sys/ioctl.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
/* io����ָ�� */
#define ON 		_IO('j',0x40)
#define OFF 		_IO('j',0x41)
#define OC_RST 	_IO('j',0x42)
/* io״̬�ṹ�� */
struct io_val_t {
	int K1;
	int K2;
	int RST;
	int OC;
};
/* ͨ���� */
#define DEVNUM 5
/* ͨ���ļ������� */
int juns[DEVNUM];
/* �Ӳ�ͨ����ʼ������ */
int junc_init();