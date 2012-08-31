/*
 * Simple - REALLY simple memory mapping demonstration. 
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>   /* printk() */
#include <linux/slab.h>   /* kmalloc() */
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/types.h>    /* size_t */
#include <linux/mm.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#define pGPG3CON 0xE03001C0
#define pGPG3DAT 0xE03001C4
static void *vGPG3CON , *vGPG3DAT;
#define GPG3CON (*(volatile unsigned int *) vGPG3CON)
#define GPG3DAT (*(volatile unsigned int *) vGPG3DAT)
#define LED_ON 0x4800
#define LED_OFF 0x4801

static int simple_major = 250;
module_param(simple_major, int, 0);
MODULE_AUTHOR("farsight");
MODULE_LICENSE("Dual BSD/GPL");

//static int flag = 0;

/*
 * Open the device; in fact, there's nothing to do here.
 */
int simple_open (struct inode *inode, struct file *filp)
{
	vGPG3CON=ioremap(pGPG3CON,0x10);
	vGPG3DAT=vGPG3CON+0x04;
	GPG3CON=0x1111;
	GPG3DAT&=0xf0;
	return 0;
}

ssize_t simple_read(struct file *file, char __user *buff, size_t count, loff_t *offp)
{
	return 0;
}

ssize_t simple_write(struct file *file, const char __user *buff, size_t count, loff_t *offp)
{
	return 0;
}

inline void led_off(int arg )
{
	GPG3DAT=GPG3DAT&(~(1<<arg));
	//printk("stop led\n");
}

inline void led_on(int arg)
{
	GPG3DAT=GPG3DAT|(1<<arg);
	//printk("start led\n");
}

static int simple_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	if(arg < 0 || arg > 3)
	{
		printk("operate register error,should be 0~3.\n");
		return 0;
	}
	switch ( cmd ) 
	{
		case LED_ON:
			{
				led_on(arg);
				break;
			}
		case LED_OFF:
			{
				led_off(arg);
				break;
			}
		default:
			{
				break;
			}
	}
	return 0;
}

static int simple_release(struct inode *node, struct file *file)
{
	return 0;
}

/*
 * Set up the cdev structure for a device.
 */
static void simple_setup_cdev(struct cdev *dev, int minor,
		struct file_operations *fops)
{
	int err, devno = MKDEV(simple_major, minor);

	cdev_init(dev, fops);
	dev->owner = THIS_MODULE;
	dev->ops = fops;
	err = cdev_add (dev, devno, 1);
	/* Fail gracefully if need be */
	if (err)
		printk (KERN_NOTICE "Error %d adding simple%d", err, minor);
}


/*
 * Our various sub-devices.
 */
/* Device 0 uses remap_pfn_range */
static struct file_operations simple_remap_ops = {
	.owner   = THIS_MODULE,
	.open    = simple_open,
	.release = simple_release,
	.read    = simple_read,
	.write   = simple_write,
	.ioctl   = simple_ioctl,	
};

/*
 * We export two simple devices.  There's no need for us to maintain any
 * special housekeeping info, so we just deal with raw cdevs.
 */
static struct cdev SimpleDevs;

/*
 * Module housekeeping.
 */
static struct class *my_class;
static int simple_init(void)
{
	int result;
	dev_t dev = MKDEV(simple_major, 0);

	/* Figure out our device number. */
	if (simple_major)
		result = register_chrdev_region(dev, 1, "simple");
	else {
		result = alloc_chrdev_region(&dev, 0, 1, "simple");
		simple_major = MAJOR(dev);
	}
	if (result < 0) {
		printk(KERN_WARNING "simple: unable to get major %d\n", simple_major);
		return result;
	}
	if (simple_major == 0)
		simple_major = result;

	/* Now set up two cdevs. */
	simple_setup_cdev(&SimpleDevs, 0, &simple_remap_ops);
	printk("simple device installed, with major %d\n", simple_major);
	my_class= class_create(THIS_MODULE, "led");
	device_create(my_class, NULL, MKDEV(simple_major, 0),
		      NULL, "led");
	return 0;
}


static void simple_cleanup(void)
{
	cdev_del(&SimpleDevs);
	unregister_chrdev_region(MKDEV(simple_major, 0), 1);
//	device_destroy(my_class,MKDEV(simple_major,0));
	printk("simple device uninstalled\n");
}


module_init(simple_init);
module_exit(simple_cleanup);
