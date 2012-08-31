/*
 * using pwm timer1 to driver beep on FS_S5PC100
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/timer.h>
#include <linux/delay.h>
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

//#include <plat/regs-timer.h>	// s5pc100 pwm timer
//#include <mach/map.h>	// define hardware register address
#include <mach/gpio.h>
#include <plat/gpio-bank-d.h>
#include <plat/gpio-cfg.h>

#define	S5PC1XX_TIMER_BASE	0xEA000000
#define MAP_SIZE	0x48
#define TCFG0_OFFSET	0x0
#define TCFG1_OFFSET	0x4
#define TCON_OFFSET		0x8
#define TCNTB1_OFFSET	0x18
#define TCMPB1_OFFSET	0x1C

#define rGPDCON		0xE0300080
#define rGPDDAT		0xE0300084

#define BEEP_ON		_IO('c', 1)
#define BEEP_OFF	_IO('c', 2)

static void __iomem		*base_addr;
static void __iomem		*gpdcon;
static void __iomem		*gpddat;

static struct timer_list timer;
static int timer_count = 0;

static int beep_major = 0;
module_param( beep_major, int, 0 );
MODULE_LICENSE( "GPL" );

struct class *my_class;

/*
static void beep_timer_handler( unsigned long data )
{
	if( timer_count & 0x1 )
		writel( readl(gpddat) | (0x1<<1), gpddat );
	else 
		writel( readl(gpddat) & ~(0x1<<1), gpddat );

	printk( KERN_INFO "GPD1 = %d\n", readl(gpddat) & (0x1<<1) );

	timer_count++;
	mod_timer( &timer, jiffies + HZ/10 );
}
*/
static int beep_open( struct inode *inode, struct file *filp )
{
	int ret;
		printk( KERN_ERR "go to beep open\n");

	/* do ioremap  */
	base_addr = ioremap( S5PC1XX_TIMER_BASE, MAP_SIZE );	
	if( NULL == base_addr ) {
		printk( KERN_ERR "Failed to remap mem region for s5pc100 timer\n" );
		ret = -ENOMEM;
		return ret;
	}

	/* Initialize pwm timer1 */
	// Set prescaler for timer 1, 0xff
	writel( readl(base_addr+TCFG0_OFFSET) | 0xFF, base_addr+TCFG0_OFFSET );	
	// Set divider mux for timer 1, 1/8
	writel( (readl(base_addr+TCFG1_OFFSET) & ~(0xF<<4)) | (0x3 << 4), base_addr+TCFG1_OFFSET );
	writel( 0x1000, base_addr + TCNTB1_OFFSET );
	writel( 0x500, base_addr + TCMPB1_OFFSET );
	// Timer1: Stop; Manual Update : 1; Output Inverter On; Auto Reload On
	writel( (readl(base_addr+TCON_OFFSET) & ~(0xF<<8)) | (0xE << 8), base_addr+TCON_OFFSET );
	//writel( 0x1000, base_addr + TCNTB1_OFFSET );
	//writel( 0x500, base_addr + TCMPB1_OFFSET );

	printk( KERN_INFO "TCNTB1 = 0x%x\n", readl(base_addr+TCNTB1_OFFSET) );
	printk( KERN_INFO "TCMPB1 = 0x%x\n", readl(base_addr+TCMPB1_OFFSET) );
	printk( KERN_INFO "TCON = 0x%x\n", readl(base_addr+TCON_OFFSET) );
	printk( KERN_INFO "TCFG0 = 0x%x\n", readl(base_addr+TCFG0_OFFSET) );
	printk( KERN_INFO "TCFG1 = 0x%x\n", readl(base_addr+TCFG1_OFFSET) );

	/* Config GPD1 as Tout1 */
	s3c_gpio_cfgpin( S5PC1XX_GPD(1), S5PC1XX_GPD1_TOUT_1);
	//s3c_gpio_cfgpin( S5PC1XX_GPD(1), S5PC1XX_GPD_OUTPUT(1) );
	//gpdcon = ioremap( rGPDCON, 0x4 );
	//gpddat = ioremap( rGPDDAT, 0x4 );
	//printk( KERN_INFO "gpdcon = 0x%x", readl(gpdcon) );

	//add_timer( &timer );

	return 0;
}

static int beep_close( struct inode *inode, struct file *filp )
{
	/* Release resource */
		printk( KERN_ERR "go to beep open\n");
	iounmap( base_addr );
	return 0;
}

static int 
beep_ioctl(struct inode *inode, struct file *file, 
		unsigned int cmd)
{
	int i;

	switch( cmd ) {
		case BEEP_ON:
	printk( KERN_ERR "Enter beep ioctl\n" );
			// Start Timer1
			writel( (readl(base_addr+TCON_OFFSET) & ~(0xF<<8)) | (0xD << 8), base_addr+TCON_OFFSET );
			printk( KERN_INFO "Beep On\n" );
			break;
		case BEEP_OFF:
			//Stop Timer1
            //writel((readl(base_addr+TCON_OFFSET) & 0),base_addr+TCFG0_OFFSET);			
	printk( KERN_ERR "Enter beep ioctl\n" );
	
            writel((readl(base_addr+TCON_OFFSET) & (0x0<<8)),base_addr+TCON_OFFSET);			
			printk( KERN_INFO "Beep Off\n" );
			break;
		default:
			break;
	}

	return 0;
}

static struct file_operations beep_fops = { 
	.owner   = THIS_MODULE,
	.open    = beep_open,
	.release = beep_close,
	.ioctl   = beep_ioctl,
};

static int __init beep_init( void )
{
	int res;
			printk ( KERN_INFO "major init\n");

	res = register_chrdev( beep_major, "beep", &beep_fops );

        printk (KERN_INFO "beep_major = %d\n",beep_major);

	if ( res < 0 ) { 
		printk( KERN_ERR "%s failed.\n", __FUNCTION__ );
		return res;
	}   
	else {
		if ( beep_major )
			printk ( KERN_INFO "major number of beep is %d\n", beep_major );
		else {
			beep_major = res;
			printk ( KERN_INFO "major number of beep is %d\n", beep_major );
		}   
	}   

        printk (KERN_INFO "beep_major = %d\n",beep_major);
    my_class = class_create(THIS_MODULE, "mybeep");
    device_create(my_class,NULL,MKDEV(beep_major,0),NULL,"beep");

	//init_timer( &timer );
	//timer.function = beep_timer_handler;
	//timer.expires = jiffies + HZ / 10;

	return 0;
}

static void __exit beep_cleanup( void )
{
	unregister_chrdev( beep_major, "drv_beep" );
    device_destroy(my_class,MKDEV(beep_major,0));
    class_destroy(my_class);
}

module_init( beep_init );
module_exit( beep_cleanup );

