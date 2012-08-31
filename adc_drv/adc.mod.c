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
	{ 0x58ea7f73, "clk_get" },
	{ 0x76f69ac6, "device_create" },
	{ 0x883a754, "__class_create" },
	{ 0x4cc81886, "cdev_add" },
	{ 0xa7a86a88, "cdev_init" },
	{ 0xea147363, "printk" },
	{ 0xd8e484f0, "register_chrdev_region" },
	{ 0x67c2fa54, "__copy_to_user" },
	{ 0xf82084f9, "class_destroy" },
	{ 0xf3780734, "device_destroy" },
	{ 0xcde03413, "clk_enable" },
	{ 0x45a55ec8, "__iounmap" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x9f386606, "cdev_del" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "696CD4C35E57A6EA2F08C25");
