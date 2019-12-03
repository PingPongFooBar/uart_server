#include <linux/module.h>
#include <linux/platform_device.h>
#include "junction.h"

#define K1	 	(PAD_GPIO_B + 8)
#define K2		(PAD_GPIO_C + 17)
#define RST		(PAD_GPIO_C + 16)
#define OC		(PAD_GPIO_B + 18)


static struct jun_io_irq junction_data = {
	.k1 = K1,
	.k2 = K2,
	.rst = RST,
	.oc = OC,
	.oc_irq = 0
};

static struct platform_device junction_pdev = {
		.name = "junction",
		.id = 4,
		.dev = {
			.platform_data = &junction_data,
		}
};

static int __init  junction_dev_init(void)
{
	int ret;
	platform_device_register(&junction_pdev);
	return 0;
}

static void __exit junction_dev_exit(void)
{
	platform_device_unregister(&junction_pdev);
}

module_init(junction_dev_init);
module_exit(junction_dev_exit);

MODULE_AUTHOR("Chen Zejian");
MODULE_LICENSE("GPL");
