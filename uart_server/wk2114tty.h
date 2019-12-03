//����
#define FS_110592	11059200//11.0592MHz
//ȫ�ּĴ���
#define GENA  		0B000000//ʱ��ʹ��
#define GRST  		0B000001 //��λ����
#define GMUT 		0B000010 //����������λ����
#define GIER  		0B010000 //�ж�ʹ��
#define GIFR  		0B010001 //�жϱ�־

//�Ӵ��ڼĴ���
#define SPAGE0 		0B000011  //ҳ����
//ʹ��ʱע�����ڵ�ҳ
#define SCR0  		0B000100//������ʹ�ܿ���(PAGE0)
#define LCR0  		0B000101 //���ģʽ������λ����(PAGE0)
#define FCR0 		0B000110//FIFO�����㣬ʹ���Լ���λ(PAGE0)
#define SIER0 		0B000111 //�ж�ʹ�ܿ���(PAGE0)
#define SIFR0 		0B001000 //�жϱ�־ (PAGE0)
#define TFCNT0 		0B001001//����FIFO������(PAGE0)
#define RFCNT0 		0B001010 //����FIFO������(PAGE0)
#define FSR0 		0B001011 //FIFO״̬(PAGE0)
#define LSR0 			0B001100//����״̬(PAGE0)
#define FDAT0 		0B001101//FIFO���ݼĴ���(PAGE0)

#define BAUD00 		0B000101//�����ʵ��ֽ� ����(PAGE1)
#define BAUD01 		0B000100//�����ʸ��ֽ�����(PAGE1)
#define  PRES0  		0B000110//������С����(PAGE1)
#define RFTL0 		0B000111//FIFO���մ�������(PAGE1)
#define TFTL0 		0B001000//FIFO���ʹ�������(PAGE1)

//�Ӵ��ڼĴ���
#define SPAGE1 		0B010011  //ҳ����
//ʹ��ʱע�����ڵ�ҳ
#define SCR1  		0B010100//������ʹ�ܿ���(PAGE0)
#define LCR1  		0B010101 //���ģʽ������λ����(PAGE0)
#define FCR1  		0B010110 //FIFO�����㣬ʹ���Լ���λ(PAGE0)
#define SIER1 		0B010111 //�ж�ʹ�ܿ���(PAGE0)
#define SIFR1 		0B011000 //�жϱ�־ (PAGE0)
#define TFCNT1 		0B011001//����FIFO������(PAGE0)
#define RFCNT1 		0B011010 //����FIFO������(PAGE0)
#define FSR1 		0B011011 //FIFO״̬(PAGE0)
#define LSR1 			0B011100//����״̬(PAGE0)
#define FDAT1 		0B011101//FIFO���ݼĴ���(PAGE0)

#define BAUD10 		0B010101//�����ʵ��ֽ� ����(PAGE1)
#define BAUD11 		0B010100//�����ʸ��ֽ�����(PAGE1)
#define  PRES1  		0B010110//������С����(PAGE1)
#define RFTL1 		0B010111//FIFO���մ�������(PAGE1)
#define TFTL1 		0B011000//FIFO���ʹ�������(PAGE1)

//�Ӵ��ڼĴ���
#define SPAGE2 		0B100011  //ҳ����
//ʹ��ʱע�����ڵ�ҳ
#define SCR2  		0B100100//������ʹ�ܿ���(PAGE0)
#define LCR2  		0B100101 //���ģʽ������λ����(PAGE0)
#define FCR2  		0B100110 //FIFO�����㣬ʹ���Լ���λ(PAGE0)
#define SIER2 		0B100111 //�ж�ʹ�ܿ���(PAGE0)
#define SIFR2 		0B101000 //�жϱ�־ (PAGE0)
#define TFCNT2 		0B101001//����FIFO������(PAGE0)
#define RFCNT2 		0B101010 //����FIFO������(PAGE0)
#define FSR2 		0B101011 //FIFO״̬(PAGE0)
#define LSR2 			0B101100//����״̬(PAGE0)
#define FDAT2 		0B101101//FIFO���ݼĴ���(PAGE0)

#define BAUD20 		0B100101//�����ʵ��ֽ� ����(PAGE1)
#define BAUD21 		0B100100//�����ʸ��ֽ�����(PAGE1)
#define  PRES2  		0B100110//������С����(PAGE1)
#define RFTL2 		0B100111//FIFO���մ�������(PAGE1)
#define TFTL2 		0B101000//FIFO���ʹ�������(PAGE1)

//�Ӵ��ڼĴ���
#define SPAGE3 		0B110011  //ҳ����
//ʹ��ʱע�����ڵ�ҳ
#define SCR3  		0B110100//������ʹ�ܿ���(PAGE0)
#define LCR3  		0B110101 //���ģʽ������λ����(PAGE0)
#define FCR3  		0B110110 //FIFO�����㣬ʹ���Լ���λ(PAGE0)
#define SIER3 		0B110111 //�ж�ʹ�ܿ���(PAGE0)
#define SIFR3 		0B111000 //�жϱ�־ (PAGE0)
#define TFCNT3 		0B111001//����FIFO������(PAGE0)
#define RFCNT3 		0B111010 //����FIFO������(PAGE0)
#define FSR3 		0B111011 //FIFO״̬(PAGE0)
#define LSR3 			0B111100//����״̬(PAGE0)
#define FDAT3 		0B111101//FIFO���ݼĴ���(PAGE0)

#define BAUD30 		0B110101//�����ʵ��ֽ� ����(PAGE1)
#define BAUD31 		0B110100//�����ʸ��ֽ�����(PAGE1)
#define  PRES3  		0B110110//������С����(PAGE1)
#define RFTL3 		0B110111//FIFO���մ�������(PAGE1)
#define TFTL3 		0B111000//FIFO���ʹ�������(PAGE1)

//�Ӵ��ڼĴ���
#define SPAGE 	0B0011  //ҳ����
//ʹ��ʱע�����ڵ�ҳ
#define SCR  		0B0100//������ʹ�ܿ���(PAGE0)
#define LCR  		0B0101 //���ģʽ������λ����(PAGE0)
#define FCR  		0B0110 //FIFO�����㣬ʹ���Լ���λ(PAGE0)
#define SIER 		0B0111 //�ж�ʹ�ܿ���(PAGE0)
#define SIFR 		0B1000 //�жϱ�־ (PAGE0)
#define TFCNT 	0B1001//����FIFO������(PAGE0)
#define RFCNT 	0B1010 //����FIFO������(PAGE0)
#define FSR 		0B1011 //FIFO״̬(PAGE0)
#define LSR 		0B1100//����״̬(PAGE0)
#define FDAT 	0B1101//FIFO���ݼĴ���(PAGE0)

#define BAUD0 	0B0101//�����ʵ��ֽ� ����(PAGE1)
#define BAUD1 	0B0100//�����ʸ��ֽ�����(PAGE1)
#define PRES  	0B0110//������С����(PAGE1)
#define RFTL 		0B0111//FIFO���մ�������(PAGE1)
#define TFTL 		0B1000//FIFO���ʹ�������(PAGE1)

