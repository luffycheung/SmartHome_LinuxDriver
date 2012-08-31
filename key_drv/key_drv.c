#include <linux/input.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <plat/irqs.h>
#include <plat/regs-gpio.h>
#include <plat/gpio-cfg.h>
#include <plat/gpio-bank-h0.h>
#include <plat/regs-irqtype.h>
#include <mach/gpio.h>


MODULE_AUTHOR("xincn");
MODULE_LICENSE("GPL");

static int key_major = 0;
static int key;

struct class *my_class;

static irqreturn_t key1_interrupt(int irq, void *dummy)
{
//	printk(KERN_INFO "key 1 was pressed ...\n");
	key = 1;
	return IRQ_HANDLED;
}

static irqreturn_t key2_interrupt(int irq, void *dummy)
{
//	printk(KERN_INFO "key 2 was pressed ...\n");
	key = 2;
	return IRQ_HANDLED;
}

static irqreturn_t key3_interrupt(int irq, void *dummy)
{
//	printk(KERN_INFO "key 3 was pressed ...\n");
	key = 3;
	return IRQ_HANDLED;
}

static irqreturn_t key4_interrupt(int irq, void *dummy)
{
//	printk(KERN_INFO "key 4 was pressed ...\n");
	key = 4;
	return IRQ_HANDLED;
}

static irqreturn_t key5_interrupt(int irq, void *dummy)
{
//	printk(KERN_INFO "key 5 was pressed ...\n");
	key = 5;
	return IRQ_HANDLED;
}

static irqreturn_t key6_interrupt(int irq, void *dummy)
{
//	printk(KERN_INFO "key 6 was pressed ...\n");
	key = 6;
	return IRQ_HANDLED;
}

static int key_open(struct inode *inode, struct file *filp)
{
	int offs, mask, ctrl, shift;

	/* eint1 */
	s3c_gpio_cfgpin(S5PC1XX_GPH0(1), S5PC1XX_GPH0_1_WAKEUP_INT_1);
	offs = eint_offset(IRQ_EINT1);
	shift = (offs & 0x7) * 4;
	mask = 0x7 << shift;

	ctrl = __raw_readl(S5PC1XX_EINTCON(eint_conf_reg(IRQ_EINT1)));
	ctrl &= ~mask;
	ctrl |= S5P_EXTINT_FALLEDGE << shift;
	__raw_writel(ctrl, S5PC1XX_EINTCON(eint_conf_reg(IRQ_EINT1)));

	if (request_irq(IRQ_EINT1, key1_interrupt, 
				IRQF_DISABLED, "key_int", NULL))
	{
		printk(KERN_ERR "%s: Can't allocate irq %d\n", 
				__FUNCTION__, IRQ_EINT1);
	}

	/* eint2 */
	s3c_gpio_cfgpin(S5PC1XX_GPH0(2), S5PC1XX_GPH0_2_WAKEUP_INT_2);
	offs = eint_offset(IRQ_EINT2);
	shift = (offs & 0x7) * 4;
	mask = 0x7 << shift;

	ctrl = __raw_readl(S5PC1XX_EINTCON(eint_conf_reg(IRQ_EINT2)));
	ctrl &= ~mask;
	ctrl |= S5P_EXTINT_FALLEDGE << shift;
	__raw_writel(ctrl, S5PC1XX_EINTCON(eint_conf_reg(IRQ_EINT2)));

	if (request_irq(IRQ_EINT2, key2_interrupt, 
				IRQF_DISABLED, "key_int", NULL))
	{
		printk(KERN_ERR "%s: Can't allocate irq %d\n", 
				__FUNCTION__, IRQ_EINT2);
	}


	/* eint3 */
	s3c_gpio_cfgpin(S5PC1XX_GPH0(3), S5PC1XX_GPH0_3_WAKEUP_INT_3);
	offs = eint_offset(IRQ_EINT3);
	shift = (offs & 0x7) * 4;
	mask = 0x7 << shift;

	ctrl = __raw_readl(S5PC1XX_EINTCON(eint_conf_reg(IRQ_EINT3)));
	ctrl &= ~mask;
	ctrl |= S5P_EXTINT_FALLEDGE << shift;
	__raw_writel(ctrl, S5PC1XX_EINTCON(eint_conf_reg(IRQ_EINT3)));

	if (request_irq(IRQ_EINT3, key3_interrupt, 
				IRQF_DISABLED, "key_int", NULL))
	{
		printk(KERN_ERR "%s: Can't allocate irq %d\n", 
				__FUNCTION__, IRQ_EINT3);
	}

	
	/* eint4 */
	s3c_gpio_cfgpin(S5PC1XX_GPH0(4), S5PC1XX_GPH0_4_WAKEUP_INT_4);
	offs = eint_offset(IRQ_EINT4);
	shift = (offs & 0x7) * 4;
	mask = 0x7 << shift;

	ctrl = __raw_readl(S5PC1XX_EINTCON(eint_conf_reg(IRQ_EINT4)));
	ctrl &= ~mask;
	ctrl |= S5P_EXTINT_FALLEDGE << shift;
	__raw_writel(ctrl, S5PC1XX_EINTCON(eint_conf_reg(IRQ_EINT4)));

	if (request_irq(IRQ_EINT4, key4_interrupt, 
				IRQF_DISABLED, "key_int", NULL))
	{
		printk(KERN_ERR "%s: Can't allocate irq %d\n", 
				__FUNCTION__, IRQ_EINT4);
	}


	/* eint6 */
	s3c_gpio_cfgpin(S5PC1XX_GPH0(6), S5PC1XX_GPH0_6_WAKEUP_INT_6);
	offs = eint_offset(IRQ_EINT6);
	shift = (offs & 0x7) * 4;
	mask = 0x7 << shift;

	ctrl = __raw_readl(S5PC1XX_EINTCON(eint_conf_reg(IRQ_EINT6)));
	ctrl &= ~mask;
	ctrl |= S5P_EXTINT_FALLEDGE << shift;
	__raw_writel(ctrl, S5PC1XX_EINTCON(eint_conf_reg(IRQ_EINT6)));

	if (request_irq(IRQ_EINT6, key5_interrupt, 
				IRQF_DISABLED, "key_int", NULL))
	{
		printk(KERN_ERR "%s: Can't allocate irq %d\n", 
				__FUNCTION__, IRQ_EINT6);
	}



	/* eint7 */
	s3c_gpio_cfgpin(S5PC1XX_GPH0(7), S5PC1XX_GPH0_7_WAKEUP_INT_7);
	offs = eint_offset(IRQ_EINT7);
	shift = (offs & 0x7) * 4;
	mask = 0x7 << shift;

	ctrl = __raw_readl(S5PC1XX_EINTCON(eint_conf_reg(IRQ_EINT7)));
	ctrl &= ~mask;
	ctrl |= S5P_EXTINT_FALLEDGE << shift;
	__raw_writel(ctrl, S5PC1XX_EINTCON(eint_conf_reg(IRQ_EINT7)));

	if (request_irq(IRQ_EINT7, key6_interrupt, 
				IRQF_DISABLED, "key_int", NULL))
	{
		printk(KERN_ERR "%s: Can't allocate irq %d\n", 
				__FUNCTION__, IRQ_EINT7);
	}

	return 0;
}

static int key_close(struct inode *inode, struct file *filp)
{
	free_irq(IRQ_EINT1, key1_interrupt);
	free_irq(IRQ_EINT2, key2_interrupt);
	free_irq(IRQ_EINT3, key3_interrupt);
	free_irq(IRQ_EINT4, key4_interrupt);
	free_irq(IRQ_EINT6, key5_interrupt);
	free_irq(IRQ_EINT7, key6_interrupt);

	return 0;
}

static ssize_t key_read(struct file *filp, char __user *buffer, 
		size_t count, loff_t *offset)
{
	if( copy_to_user(buffer, &key, sizeof(key)) != 0 ) {
		printk( KERN_ERR "copy to user failed\n" );
		return -1;
	}
	key = 0;
	return 0;
}

static struct file_operations key_fops = {
	.owner		= THIS_MODULE,
	.open		= key_open,
	.release	= key_close,
	.read		= key_read,
};

static int __init button_init(void)
{
	int res;
	res = register_chrdev(key_major, "key", &key_fops);
	if (res < 0)
	{
		printk(KERN_ERR "%s failed.\n", __FUNCTION__);
		return res;
	}
	else
	{
		if (key_major)
			printk(KERN_INFO "major number of key is %d\n", key_major);
		else
		{
			key_major = res;
			printk(KERN_INFO "major number of key is %d\n", key_major);
		}
	}

    my_class = class_create(THIS_MODULE,"mykey");
    device_create(my_class,NULL,MKDEV(key_major,0),NULL,"key");
	
	return 0;
}

static void __exit button_cleanup(void)
{
	unregister_chrdev(key_major, "key");
    device_destroy(my_class,MKDEV(key_major,0));
    class_destroy(my_class);
}

module_init(button_init);
module_exit(button_cleanup);
