#include <sys/ioctl.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
/* io控制指令 */
#define ON 		_IO('j',0x40)
#define OFF 		_IO('j',0x41)
#define OC_RST 	_IO('j',0x42)
/* io状态结构体 */
struct io_val_t {
	int K1;
	int K2;
	int RST;
	int OC;
};
/* 通道数 */
#define DEVNUM 5
/* 通道文件描述符 */
int juns[DEVNUM];
/* 接驳通道初始化函数 */
int junc_init();