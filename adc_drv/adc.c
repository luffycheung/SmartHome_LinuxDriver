#include<linux/init.h>
#include<linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include<linux/ioctl.h>
#include<asm/uaccess.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <asm/system.h>
#include <asm/io.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/proc_fs.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/poll.h>
#include <linux/clk.h>


MODULE_LICENSE("GPL");

int adc_major = 249;
int adc_minor = 0;
int number_adc_driver = 1;
unsigned long *ADCCON_ADDR;
unsigned long *ADCDAT0_ADDR;
unsigned long *ADCMUX_ADDR;
int flags = 0;
int value;

dev_t dev;
struct cdev cdev;
struct class *my_class;
struct clk *clk;
/*ADC驱动的打开函数*/
static int adc_open(struct inode *inode,struct file *file)
{
    return 0;
}
/*ADC驱动关闭函数*/
static int adc_release(struct inode *inode,struct file *file)
{
    return 0;
}

/*读取函数*/
static int adc_read(struct file *file ,char __user *buf, size_t count ,loff_t *f_pos)
{
      value = readl(ADCDAT0_ADDR)&0xfff;//读取数据寄存器中ADC值
     if(copy_to_user(buf,(char *)&value,count))
     {
         return -EFAULT;
     }                                 //向上层传送读取的值
     writel(readl(ADCCON_ADDR)|0x1,ADCCON_ADDR);//使能控制寄存器读取下一次数值
     return value;
}


static struct file_operations adc_fops={
    .owner = THIS_MODULE,
    .open = adc_open,                        //打开设备
    .release = adc_release,                   //关闭设备
    .read = adc_read,                         //读取数值
};                                   //系统定义的结构体用于提供API


/*注册设备号*/
static void char_reg_setup_cdev(void)
{
    int error,devno = MKDEV(adc_major,adc_minor);
    cdev_init(&cdev,&adc_fops);
    cdev.owner = THIS_MODULE;
    cdev.ops  = &adc_fops;
    error = cdev_add(&cdev,devno,1);
    if(error)
    {
        printk(KERN_NOTICE"error %d adding char_reg_setup_cdev",error);

    }

    my_class = class_create(THIS_MODULE,"myadc_class"); //创建类

    if(IS_ERR(my_class))
    {
        printk("Err: failed in creating class .\n");
        return ;
    }
    device_create(my_class,NULL,devno,NULL,"mysensor");//创建设备节点
}
/*加载函数*/
static int __init adc_init(void)
{
   int result;

   dev = MKDEV(adc_major,adc_minor);


   result = register_chrdev_region(dev,number_adc_driver,"mysensor");//静态申请设备
   if(result < 0)
   {
       printk(KERN_WARNING "myled:cann't get major number %d\n" ,adc_major);
       return result;
   }
   char_reg_setup_cdev();

   clk  = clk_get(NULL,"adc");
   clk_enable(clk);                //获得系统时间并使能
    ADCCON_ADDR = ioremap(0xF3000000,4);//获得控制寄存器映射地址
    ADCDAT0_ADDR = ioremap(0xF300000C,4);//获得数据寄存器映射地址
    ADCMUX_ADDR = ioremap(0xF300001C,4);//获得管道寄存器映射地址

    writel((readl(ADCCON_ADDR)&~0xffffffff)|0x1 << 14|0x1|0x41 << 6|0x1 <<16,ADCCON_ADDR);//初始化控制寄存器
    writel(readl(ADCMUX_ADDR)&~0x0000000f,ADCMUX_ADDR);//初始化管道为0
   return 0;
}

/*卸载函数*/
static void __exit adc_exit(void)
{
    dev_t devno = MKDEV(adc_major,adc_minor);
    cdev_del (&cdev);
    unregister_chrdev_region(devno,number_adc_driver);//注销设备，设备号
    iounmap(ADCCON_ADDR);
    iounmap(ADCMUX_ADDR);
    iounmap(ADCDAT0_ADDR);//取消映射
    clk_enable(clk);//关闭时钟
    device_destroy(my_class,devno);
    class_destroy(my_class);//销毁设备节点
}

module_init(adc_init);
module_exit(adc_exit);

