#include <sys/ioctl.h>

#define ON 		_IO('j',0x40)
#define OFF 		_IO('j',0x41)
#define OC_RST 	_IO('j',0x42)

struct io_val_t {
	int K1;
	int K2;
	int RST;
	int OC;
};


#define DEVNUM 5
