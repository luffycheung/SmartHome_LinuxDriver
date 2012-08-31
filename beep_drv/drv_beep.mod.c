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
	{ 0xa5423cc4, "param_get_int" },
	{ 0xcb32da10, "param_set_int" },
	{ 0x6d4d6df5, "s3c_gpio_cfgpin" },
	{ 0x40a6f522, "__arm_ioremap" },
	{ 0x45a55ec8, "__iounmap" },
	{ 0x76f69ac6, "device_create" },
	{ 0x883a754, "__class_create" },
	{ 0xf63a2a3c, "register_chrdev" },
	{ 0xea147363, "printk" },
	{ 0xf82084f9, "class_destroy" },
	{ 0xf3780734, "device_destroy" },
	{ 0x9ef749e2, "unregister_chrdev" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "96413D0959CE5A8CD7B4E23");
