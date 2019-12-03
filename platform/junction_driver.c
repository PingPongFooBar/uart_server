#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/irq.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/string.h>
#include <linux/cdev.h>
 #include <linux/device.h>
 #include <linux/platform_device.h>
 #include <linux/kernel.h>
 #include "junction.h"

//ʹ���ں��Ƽ��Ŀ�������
//������Ԥ���������ͻ
//��ioctl-number.txt ��һ���������ڶ������к�
//�������Ѵ��ڵ��غ�
#define ON 		_IO('j',0x40)
#define OFF 		_IO('j',0x41)
#define OC_RST 	_IO('j',0x42)
#define OC_DETE	_IO('j',0x43)
#define PAD_GPIO_C 32*2
#define PAD_GPIO_B 32*1


#define DEV_NAME "junction_dev"
#define DEVNUM 5

struct io_val_t {
	int K1;
	int K2;
	int RST;
	int OC;
};

static ssize_t junction_read(struct file *,char *,size_t ,loff_t *);
static long junction_cntl (struct file *, unsigned int ,unsigned long );
static int junction_close(struct inode *inode, struct file *filp);
static int junction_open(struct inode *, struct file *);
static irqreturn_t OC_interrupt(int ,void *);
static int junction_fasync(int fd, struct file * filp, int mode);

static struct class *cls;
static dev_t major = 0;
static dev_t dev_id;
static struct cdev junction_cdev;
static struct file_operations junction_fops = {
	.owner = THIS_MODULE,
	.open = junction_open,
	.read = junction_read,
	.unlocked_ioctl = junction_cntl,
	.release = junction_close,
	.fasync = junction_fasync
};
static struct jun_io_irq junction_io_data[DEVNUM];
static int min_flag = 0;
static struct fasync_struct *async_queue;

static int  junction_probe(struct platform_device *pdev)
{
	int ret;
	char name[10] = "         ";
	int i;
	struct jun_io_irq *junction_io = dev_get_platdata(&pdev->dev);
	if(!junction_io){
		printk(KERN_EMERG"pdata pointer is null\n");
		return 0;
	}
	junction_io->oc_irq = gpio_to_irq(junction_io->oc);


	//�����豸��
	/* �ж�major �Ƿ�Ϊ0������Ƿ��һ������ */
	if(!major){
		ret = alloc_chrdev_region(&dev_id, 0, DEVNUM, "junction");
		major = MAJOR(dev_id);
		if(ret < 0){
			printk(DEV_NAME" request fail\n");
		}else{
			printk(DEV_NAME" request successfuly\n");
		}
		//��ʼ���豸
		cdev_init(&junction_cdev, &junction_fops);
		//ע���Ӧ�豸���ںˣ��Լ�Ψһʶ����豸��
		ret = cdev_add(&junction_cdev, dev_id, DEVNUM);
		if(ret < 0){
			printk(DEV_NAME" register fail\n");
		}else{
			printk(DEV_NAME" register successfuly\n");
		}
		//��sys �½���junction_class ��
		cls = class_create(THIS_MODULE, "junction_class");
	}

	for(i = 0; i < DEVNUM; i++){
		if((min_flag & (1 << i)) == 0){
			min_flag |= (1 << i);
			junction_io->minor = i;
			junction_io_data[i].k1 = junction_io->k1;
			junction_io_data[i].k2 = junction_io->k2;
			junction_io_data[i].oc = junction_io->oc;
			junction_io_data[i].rst = junction_io->rst;
			junction_io_data[i].oc_irq = junction_io->oc_irq;
			break;
		}
	}
	if(i == DEVNUM){
		printk("junction dev out of minor\n");
		return 0;
	}
	//����IO�ڣ������ж�
	//����K1
	sprintf(name, "K1_%d   ",(junction_io->minor + 1));
	gpio_free(junction_io->k1);
	ret = gpio_request(junction_io->k1,name);
	if(ret < 0){
		printk(" %s register fail\n",name);
	}else{
		printk(" %s register successfuly\n",name);
	}
	gpio_direction_output(junction_io->k1,0);
	//����K2
	sprintf(name, "K2_%d   ",(junction_io->minor + 1));
	gpio_free(junction_io->k2);
	ret = gpio_request(junction_io->k2, name);
	if(ret < 0){
		printk(" %s register fail\n",name);
	}else{
		printk(" %s register successfuly\n",name);
	}
	gpio_direction_output(junction_io->k2,0);
	//����RST
	sprintf(name, "RST_%d  ",(junction_io->minor + 1));
	gpio_free(junction_io->rst);
	ret = gpio_request(junction_io->rst,  name);
	if(ret < 0){
		printk(" %s register fail\n",name);
	}else{
		printk(" %s register successfuly\n",name);
	}
	gpio_direction_output(junction_io->rst,1);
	//����OC
	sprintf(name, "OC_%d   ",(junction_io->minor + 1));
	gpio_free(junction_io->oc);
	ret = gpio_request_one(junction_io->oc, GPIOF_IN, name);
	if(ret < 0){
		printk(" %s register fail\n",name);
	}else{
		printk(" %s register successfuly\n",name);
	}
	gpio_direction_input(junction_io->oc);
	//����OC�ж�
	sprintf(name, "OC_IRQ%d",(junction_io->minor + 1));
	ret = request_irq(junction_io->oc_irq, OC_interrupt,IRQF_TRIGGER_RISING, 
                         name, NULL);
	if(ret)
	{
		free_irq(junction_io->oc_irq,NULL);
		return -EBUSY;
	}
	irq_set_irq_type(junction_io->oc_irq,IRQ_TYPE_EDGE_RISING);
	disable_irq(junction_io->oc_irq);
	enable_irq(junction_io->oc_irq);

	// ���ִ����豸�ļ��ķ����õ���udev ����
	//	�����豸�����ͻ���sysfs �ļ����²������junction_class ·��
	/* �����洴���� */
	//�����豸�ļ�
	//����junction_class �²����ӽڵ㣬���Զ�����uevent ����netlink ��Ϣ
	sprintf(name, "junction%d",(junction_io->minor + 1));
	device_create(cls, NULL, MKDEV(major, junction_io->minor), NULL, name);
	printk("junction_driver initailized\n");
	return 0;
}

static int  junction_remove(struct platform_device *pdev)
{
	struct jun_io_irq *junction_io = dev_get_platdata(&pdev->dev);
	//ɾ���豸�ڵ�
	device_destroy(cls, MKDEV(major, junction_io->minor));
	min_flag &= ~(1 << junction_io->minor);

	//�ͷ�IO�ں��ж�
	gpio_free(junction_io->k1);
	gpio_free(junction_io->k2);
	gpio_free(junction_io->rst);
	gpio_free(junction_io->oc);
	disable_irq(junction_io->oc_irq);
	free_irq(junction_io->oc_irq,NULL);
	
	if(min_flag & 0b00000){
		class_destroy(cls);
		//ע������
		cdev_del(&junction_cdev);
		//ע��������
		 unregister_chrdev_region(dev_id, DEVNUM);
		 printk(DEV_NAME" unregister successfuly\n");
	}
	return 0;
}


static int junction_open(struct inode *inode, struct file *filp)
{
	printk(KERN_EMERG"on:%d,off:%d,rst:%d\n",ON,OFF,OC_RST);
	int minor;
	//��ô��豸��
	 minor = MINOR(inode->i_rdev);
	return 0;
}

static ssize_t junction_read(struct file *filp, char *buf, size_t size, loff_t *off)
{
	int n;
	int minor;
	static struct io_val_t io_val;
	n = sizeof(io_val);
	struct inode *inode = filp->f_path.dentry->d_inode;
	minor = MINOR(inode->i_rdev);
	io_val.K1 	= gpio_get_value(junction_io_data[minor].k1);
	io_val.K2 	= gpio_get_value(junction_io_data[minor].k2);
	io_val.RST	= gpio_get_value(junction_io_data[minor].rst);
	io_val.OC	= gpio_get_value(junction_io_data[minor].oc);
	copy_to_user(buf,&io_val,n);
	return n;
}

static long junction_cntl (struct file *file, unsigned int cmd,unsigned long arg)
{
	//��ô��豸��
	struct inode *inode = file->f_path.dentry->d_inode;
	int  minor = MINOR(inode->i_rdev);
	printk(KERN_EMERG"cmd:%d\n",cmd);
	 //����ָ����в���
	 switch(cmd){
		case ON:
			if(gpio_get_value(junction_io_data[minor].oc)){
				printk(KERN_EMERG"over current, cannot turn on\n");
				break;
			}
			gpio_set_value(junction_io_data[minor].k1,1);//�̵���
			msleep(300);
			gpio_set_value(junction_io_data[minor].k2,1);//MOS��
			break;
		case OFF:
			gpio_set_value(junction_io_data[minor].k2,0);//MOS��
			msleep(300);
			gpio_set_value(junction_io_data[minor].k1,0);//�̵���
			break;
		case OC_RST:
			gpio_set_value(junction_io_data[minor].rst,0);
			msleep(500);
			gpio_set_value(junction_io_data[minor].rst,1);
			break;
		default:
			printk("cmd erro\n");
			return -1;
	 }
	 return 0;
}

static int junction_fasync(int fd, struct file * filp, int mode){
	return fasync_helper(fd, filp, mode, &async_queue);
}

static int junction_close(struct inode *inode, struct file *filp)
{
	int minor;
	//��ô��豸��
	minor = MINOR(inode->i_rdev);
	//disable_irq(junction_io_data[minor].oc_irq);
	junction_fasync(-1,filp,0);
	return 0;
}

static irqreturn_t OC_interrupt(int irq,void *dev_id)
{
	printk(KERN_EMERG"over current,irq:%d\n",irq);
	kill_fasync(&async_queue,SIGIO, POLL_IN);
	return IRQ_RETVAL(IRQ_HANDLED);
}

static struct platform_driver junction_driver = {
	.driver = {
		.name = "junction",
		.owner = THIS_MODULE,
	},
	.probe = junction_probe,
	.remove = junction_remove,
};

module_platform_driver(junction_driver);

MODULE_AUTHOR("Chen Zejian");
MODULE_LICENSE("GPL");
