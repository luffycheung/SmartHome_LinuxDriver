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
	{ 0x67c2fa54, "__copy_to_user" },
	{ 0x1fc91fb2, "request_irq" },
	{ 0x6d4d6df5, "s3c_gpio_cfgpin" },
	{ 0xf20dabd8, "free_irq" },
	{ 0x76f69ac6, "device_create" },
	{ 0x883a754, "__class_create" },
	{ 0xea147363, "printk" },
	{ 0xf63a2a3c, "register_chrdev" },
	{ 0xf82084f9, "class_destroy" },
	{ 0xf3780734, "device_destroy" },
	{ 0x9ef749e2, "unregister_chrdev" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "8E1B7AB03808917963071D0");
