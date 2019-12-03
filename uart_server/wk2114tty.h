//晶振
#define FS_110592	11059200//11.0592MHz
//全局寄存器
#define GENA  		0B000000//时钟使能
#define GRST  		0B000001 //复位控制
#define GMUT 		0B000010 //主串口数据位控制
#define GIER  		0B010000 //中断使能
#define GIFR  		0B010001 //中断标志

//子串口寄存器
#define SPAGE0 		0B000011  //页控制
//使用时注意所在的页
#define SCR0  		0B000100//休眠与使能控制(PAGE0)
#define LCR0  		0B000101 //输出模式与数据位控制(PAGE0)
#define FCR0 		0B000110//FIFO触发点，使能以及复位(PAGE0)
#define SIER0 		0B000111 //中断使能控制(PAGE0)
#define SIFR0 		0B001000 //中断标志 (PAGE0)
#define TFCNT0 		0B001001//发送FIFO计数器(PAGE0)
#define RFCNT0 		0B001010 //接收FIFO计数器(PAGE0)
#define FSR0 		0B001011 //FIFO状态(PAGE0)
#define LSR0 			0B001100//接收状态(PAGE0)
#define FDAT0 		0B001101//FIFO数据寄存器(PAGE0)

#define BAUD00 		0B000101//比特率低字节 设置(PAGE1)
#define BAUD01 		0B000100//波特率高字节设置(PAGE1)
#define  PRES0  		0B000110//波特率小数点(PAGE1)
#define RFTL0 		0B000111//FIFO接收触发控制(PAGE1)
#define TFTL0 		0B001000//FIFO发送触发控制(PAGE1)

//子串口寄存器
#define SPAGE1 		0B010011  //页控制
//使用时注意所在的页
#define SCR1  		0B010100//休眠与使能控制(PAGE0)
#define LCR1  		0B010101 //输出模式与数据位控制(PAGE0)
#define FCR1  		0B010110 //FIFO触发点，使能以及复位(PAGE0)
#define SIER1 		0B010111 //中断使能控制(PAGE0)
#define SIFR1 		0B011000 //中断标志 (PAGE0)
#define TFCNT1 		0B011001//发送FIFO计数器(PAGE0)
#define RFCNT1 		0B011010 //接收FIFO计数器(PAGE0)
#define FSR1 		0B011011 //FIFO状态(PAGE0)
#define LSR1 			0B011100//接收状态(PAGE0)
#define FDAT1 		0B011101//FIFO数据寄存器(PAGE0)

#define BAUD10 		0B010101//比特率低字节 设置(PAGE1)
#define BAUD11 		0B010100//波特率高字节设置(PAGE1)
#define  PRES1  		0B010110//波特率小数点(PAGE1)
#define RFTL1 		0B010111//FIFO接收触发控制(PAGE1)
#define TFTL1 		0B011000//FIFO发送触发控制(PAGE1)

//子串口寄存器
#define SPAGE2 		0B100011  //页控制
//使用时注意所在的页
#define SCR2  		0B100100//休眠与使能控制(PAGE0)
#define LCR2  		0B100101 //输出模式与数据位控制(PAGE0)
#define FCR2  		0B100110 //FIFO触发点，使能以及复位(PAGE0)
#define SIER2 		0B100111 //中断使能控制(PAGE0)
#define SIFR2 		0B101000 //中断标志 (PAGE0)
#define TFCNT2 		0B101001//发送FIFO计数器(PAGE0)
#define RFCNT2 		0B101010 //接收FIFO计数器(PAGE0)
#define FSR2 		0B101011 //FIFO状态(PAGE0)
#define LSR2 			0B101100//接收状态(PAGE0)
#define FDAT2 		0B101101//FIFO数据寄存器(PAGE0)

#define BAUD20 		0B100101//比特率低字节 设置(PAGE1)
#define BAUD21 		0B100100//波特率高字节设置(PAGE1)
#define  PRES2  		0B100110//波特率小数点(PAGE1)
#define RFTL2 		0B100111//FIFO接收触发控制(PAGE1)
#define TFTL2 		0B101000//FIFO发送触发控制(PAGE1)

//子串口寄存器
#define SPAGE3 		0B110011  //页控制
//使用时注意所在的页
#define SCR3  		0B110100//休眠与使能控制(PAGE0)
#define LCR3  		0B110101 //输出模式与数据位控制(PAGE0)
#define FCR3  		0B110110 //FIFO触发点，使能以及复位(PAGE0)
#define SIER3 		0B110111 //中断使能控制(PAGE0)
#define SIFR3 		0B111000 //中断标志 (PAGE0)
#define TFCNT3 		0B111001//发送FIFO计数器(PAGE0)
#define RFCNT3 		0B111010 //接收FIFO计数器(PAGE0)
#define FSR3 		0B111011 //FIFO状态(PAGE0)
#define LSR3 			0B111100//接收状态(PAGE0)
#define FDAT3 		0B111101//FIFO数据寄存器(PAGE0)

#define BAUD30 		0B110101//比特率低字节 设置(PAGE1)
#define BAUD31 		0B110100//波特率高字节设置(PAGE1)
#define  PRES3  		0B110110//波特率小数点(PAGE1)
#define RFTL3 		0B110111//FIFO接收触发控制(PAGE1)
#define TFTL3 		0B111000//FIFO发送触发控制(PAGE1)

//子串口寄存器
#define SPAGE 	0B0011  //页控制
//使用时注意所在的页
#define SCR  		0B0100//休眠与使能控制(PAGE0)
#define LCR  		0B0101 //输出模式与数据位控制(PAGE0)
#define FCR  		0B0110 //FIFO触发点，使能以及复位(PAGE0)
#define SIER 		0B0111 //中断使能控制(PAGE0)
#define SIFR 		0B1000 //中断标志 (PAGE0)
#define TFCNT 	0B1001//发送FIFO计数器(PAGE0)
#define RFCNT 	0B1010 //接收FIFO计数器(PAGE0)
#define FSR 		0B1011 //FIFO状态(PAGE0)
#define LSR 		0B1100//接收状态(PAGE0)
#define FDAT 	0B1101//FIFO数据寄存器(PAGE0)

#define BAUD0 	0B0101//比特率低字节 设置(PAGE1)
#define BAUD1 	0B0100//波特率高字节设置(PAGE1)
#define PRES  	0B0110//波特率小数点(PAGE1)
#define RFTL 		0B0111//FIFO接收触发控制(PAGE1)
#define TFTL 		0B1000//FIFO发送触发控制(PAGE1)

