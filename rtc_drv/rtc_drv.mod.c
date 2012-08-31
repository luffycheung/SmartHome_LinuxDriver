#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x8c5f194b, "struct_module" },
	{ 0x40a6f522, "__arm_ioremap" },
	{ 0xfbc74f64, "__copy_from_user" },
	{ 0x67c2fa54, "__copy_to_user" },
	{ 0xfec3c2f2, "bcd2bin" },
	{ 0x5edd0762, "bin2bcd" },
	{ 0x8f01d2c6, "_spin_unlock" },
	{ 0x2c627b89, "_spin_lock" },
	{ 0x45a55ec8, "__iounmap" },
	{ 0x34c5bede, "malloc_sizes" },
	{ 0x76f69ac6, "device_create" },
	{ 0x883a754, "__class_create" },
	{ 0xc3fe6db4, "__spin_lock_init" },
	{ 0x4cc81886, "cdev_add" },
	{ 0xa7a86a88, "cdev_init" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x3b4ffc12, "kmem_cache_alloc" },
	{ 0xea147363, "printk" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0xd8e484f0, "register_chrdev_region" },
	{ 0xf3780734, "device_destroy" },
	{ 0x37a0cba, "kfree" },
	{ 0x7485e15e, "unregister_chrdev_region" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "973715CFA864B40619858EA");
