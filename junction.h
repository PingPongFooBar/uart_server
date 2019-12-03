#define PAD_GPIO_C 32*2
#define PAD_GPIO_B 32*1

//IO名分组
#define K1_1 	(PAD_GPIO_C + 7)
#define K2_1		(PAD_GPIO_C + 24)
#define RST_1	(PAD_GPIO_C + 8)
#define OC_1		(PAD_GPIO_C + 28)

#define K1_2 	(PAD_GPIO_C + 0)
#define K2_2		(PAD_GPIO_C + 1)
#define RST_2	(PAD_GPIO_C + 2)
#define OC_2		(PAD_GPIO_C + 3)

#define K1_3 	(PAD_GPIO_B + 24)
#define K2_3		(PAD_GPIO_B + 9)
#define RST_3	(PAD_GPIO_B + 26)
#define OC_3		(PAD_GPIO_C + 4)

#define K1_4 	(PAD_GPIO_B + 14)
#define K2_4		(PAD_GPIO_B + 30)
#define RST_4	(PAD_GPIO_C + 15)
#define OC_4		(PAD_GPIO_B + 31)

#define K1_5 	(PAD_GPIO_B + 8)
#define K2_5		(PAD_GPIO_C + 17)
#define RST_5	(PAD_GPIO_C + 16)
#define OC_5		(PAD_GPIO_B + 18)
/*第六组需要再用*/
/*
#define K1_6 	(PAD_GPIO_C + 9)
#define K2_6		(PAD_GPIO_C + 10)
#define RST_6	(PAD_GPIO_C + 11)
#define OC_6		(PAD_GPIO_C + 12)
*/

struct jun_io_irq {
	int k1;
	int k2;
	int rst;
	int oc;
	int oc_irq;
	int minor;
};
