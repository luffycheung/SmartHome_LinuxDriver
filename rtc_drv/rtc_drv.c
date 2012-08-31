#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>//class_create

#include <linux/kernel.h>//printk
#include <linux/slab.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>//copy_to_user
#include <asm/io.h>
#include <linux/bcd.h>//bcd2bin
#include <linux/ide.h>

#define READ_TIME _IO('Z', 0)
#define SET_TIME _IO('Z', 1)

#define pRTCCON 0XEA300040
#define pBCDSEC 0XEA300070

#define BCDSEC_OFFSET 0X0
#define BCDMIN_OFFSET 0X4 
#define BCDHOUR_OFFSET 0X8
#define BCDDATE_OFFSET 0XC//day
#define BCDDAY_OFFSET 0X10
#define BCDMON_OFFSET 0X14
#define BCDYEAR_OFFSET 0X18
#define IOMAP_MAX 0X20

static int rtc_major = 247;
static struct class *my_class;
static void __iomem *vBCDSEC;
static void __iomem *vRTCCON;

spinlock_t rtc_lock;
struct rtc_dev {
	struct cdev cdev;
	int count;
};

struct rtc_tm {
	int sec;
	int min;
	int hour;
	int date;
	int mon;
	int year;
}tm;

struct rtc_dev *rtc_device;

int rtc_register_init(void)//register ioremap
{
	vRTCCON = ioremap(pRTCCON, 0x4);
	vBCDSEC = ioremap(pBCDSEC, 0x40);
	if(vBCDSEC == NULL && vRTCCON == NULL)
	{
		return -ENOMEM;
		printk(KERN_NOTICE "address ioremap error\n");
	}
	return 0;
}

void rtc_gettm(void)//readb BCDregister to get data of time
{
	unsigned int have_retried = 0;
retry_rtc_gettm:
	tm.sec = readb(vBCDSEC);
	tm.min = readb(vBCDSEC+BCDMIN_OFFSET);
	tm.hour = readb(vBCDSEC+BCDHOUR_OFFSET);
	tm.date = readb(vBCDSEC+BCDDATE_OFFSET);
	tm.mon = readb(vBCDSEC+BCDMON_OFFSET);
	tm.year = readb(vBCDSEC+BCDYEAR_OFFSET);
	if(tm.sec == 0 && !have_retried)
	{
		have_retried = 1;
		goto retry_rtc_gettm;
	}
	tm.sec = bcd2bin(tm.sec);
	tm.min = bcd2bin(tm.min);
	tm.hour = bcd2bin(tm.hour);
	tm.date = bcd2bin(tm.date);
	tm.mon = bcd2bin(tm.mon);
	tm.year = bcd2bin(tm.year);
	


//	printk(KERN_NOTICE "year/mon/day:%02x/%02x/%02x/ hour:min:sec:%02x:%02x:%02x\n",\
			tm.year, tm.mon, tm.date, tm.hour, tm.min, tm.sec);

}

void rtc_settm(void)//set the time
{
	writeb((readb(vRTCCON) & ~(1<<0)) | 1<<0, vRTCCON);
	printk(KERN_NOTICE "rtccon:%x\n", readb(vRTCCON));
	
	tm.year = bin2bcd(tm.year);
	tm.mon = bin2bcd(tm.mon);
	tm.date = bin2bcd(tm.date);
	tm.hour = bin2bcd(tm.hour);
	tm.min = bin2bcd(tm.min);
	tm.sec = bin2bcd(tm.sec);
	writeb((readb(vBCDSEC+BCDYEAR_OFFSET) & ~(0XFF))| tm.year, vBCDSEC+BCDYEAR_OFFSET);//year
	writeb((readb(vBCDSEC+BCDMON_OFFSET) & ~(0X1F))| tm.mon, vBCDSEC+BCDMON_OFFSET);//month
	writeb((readb(vBCDSEC+BCDDATE_OFFSET) & ~(0X3F))| tm.date, vBCDSEC+BCDDATE_OFFSET);//day
	writeb((readb(vBCDSEC+BCDHOUR_OFFSET) & ~(0X3F))| tm.hour, vBCDSEC+BCDHOUR_OFFSET);//hour
	writeb((readb(vBCDSEC+BCDMIN_OFFSET) & ~(0X7F))| tm.min, vBCDSEC+BCDMIN_OFFSET);//minute
	writeb((readb(vBCDSEC) & ~(0X7F))| tm.sec, vBCDSEC);//second
		
//	printk(KERN_NOTICE "year/mon/day:%02x/%02x/%02x/ hour:min:sec:%02x:%02x:%02x\n",\
			readb(vBCDSEC+BCDYEAR_OFFSET), readb(vBCDSEC+BCDMON_OFFSET), readb(vBCDSEC+BCDDATE_OFFSET),\
			readb(vBCDSEC+BCDHOUR_OFFSET), readb(vBCDSEC+BCDMIN_OFFSET),readb(vBCDSEC));
}

static int rtc_open(struct inode *inode, struct file *filp)
{
	struct rtc_dev *dev;
	dev = container_of(inode->i_cdev, struct rtc_dev, cdev);
	filp->private_data = dev;
	spin_lock (&rtc_lock);
	if(rtc_device->count)
	{
		spin_unlock(&rtc_lock);
		return -EBUSY;
	}
	rtc_device->count++;
	spin_unlock(&rtc_lock);
		
	rtc_register_init();
	printk("rtc_drv open sucess\n");
	return 0;
}

static void rtc_release(struct inode *inode, struct file *filp)
{
	struct rtc_dev *dev;
	dev = filp->private_data;
	iounmap(vRTCCON);
	iounmap(vBCDSEC);
	spin_lock(&rtc_lock);
	rtc_device->count--;
	spin_unlock(&rtc_lock);
	printk("rtc_drv release sucess\n");
}

static int rtc_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk(KERN_NOTICE "enter ioctl\n");
	struct rtc_dev *dev;
	dev = filp->private_data;
	void __user *uarg = (void __user *)arg;
	switch(cmd)
	{
	case READ_TIME:
		rtc_gettm();
		printk(KERN_NOTICE "READ_TIME:  %02x:%02x:%02x\n", tm.hour, tm.min, tm.sec);
		if(copy_to_user(uarg, &tm, sizeof(tm)))
			return -EFAULT;
		break;
	case SET_TIME:
		if(copy_from_user(&tm, uarg, sizeof(tm)))
			return -EFAULT;
			printk(KERN_NOTICE "SET_TIME sec %02x:%02x:%02x %02x:%02x:%02x\n", tm.year, tm.mon, tm.date,\
					tm.hour, tm.min, tm.sec);
			rtc_settm();
		break;
	default:
		break;
	}
	return 0;
}

static struct file_operations rtc_fops = {
	.owner = THIS_MODULE,
	.open = rtc_open,
	.release = rtc_release,
	.ioctl = rtc_ioctl,
};

static void rtc_setup_cdev(void)
{
	int err, devno = MKDEV(rtc_major, 0);

	cdev_init(&rtc_device->cdev, &rtc_fops);
	rtc_device->cdev.owner = THIS_MODULE;
	rtc_device->cdev.ops = &rtc_fops;
	err = cdev_add(&rtc_device->cdev, devno, 1);
	if(err)
		printk(KERN_NOTICE "Error adding rtc:%d\n", err);
}

static int __init rtc_init(void)
{
	int ret;
	dev_t devno = MKDEV(rtc_major, 0);
	if(rtc_major)
		ret = register_chrdev_region(devno, 1, "myrtc");
	else
	{
		ret = alloc_chrdev_region(&devno, 0, 1, "myrtc");
		rtc_major = MAJOR(devno);
	}

	if(ret < 0) {
		printk(KERN_ERR "%s failed.\n", __FUNCTION__ );
		return ret;
	}
	
	rtc_device = kmalloc(sizeof(struct rtc_dev), GFP_KERNEL);
	if(!rtc_device)
	{
		ret = -ENOMEM;
		goto fail_malloc;
	}

	memset(rtc_device, 0, sizeof(struct rtc_dev));
	rtc_setup_cdev();
	spin_lock_init(&rtc_lock);
	my_class = class_create(THIS_MODULE, "myrtc");
	device_create(my_class, NULL, MKDEV(rtc_major, 0), NULL, "myrtc");
	
	/*
	int secold = 0;
	while(1)
	{
		do 
		{
			rtc_gettm();
		}while (tm.sec == secold);
		secold = tm.sec;
   		printk(KERN_NOTICE "year/mon/day:%02x/%02x/%02x/ hour:min:sec:%02x:%02x:%02x\n",\
				tm.year, tm.mon, tm.date, tm.hour, tm.min, tm.sec);
	}*/
	printk("module_init sucess\n");	
	return 0;
fail_malloc:
	unregister_chrdev_region(devno, 1);
	return ret;
}

static void __exit rtc_exit(void)
{
	dev_t devno = MKDEV(rtc_major, 0);
	unregister_chrdev_region(devno, 1);
	kfree(rtc_device);
	device_destroy(my_class, devno);
}

module_init(rtc_init);
module_exit(rtc_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("farsight");

