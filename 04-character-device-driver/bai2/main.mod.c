#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xcf52de0f, "module_layout" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x5e340a29, "class_destroy" },
	{ 0x3c62d614, "device_destroy" },
	{ 0x200fb5f7, "cdev_del" },
	{ 0x911ab939, "device_create" },
	{ 0xff3f4f61, "__class_create" },
	{ 0x865d0657, "cdev_add" },
	{ 0xc3f36a4f, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x8a49679f, "gpiod_direction_output" },
	{ 0x193b177c, "gpio_to_desc" },
	{ 0x8da6585d, "__stack_chk_fail" },
	{ 0xec3d2e1b, "trace_hardirqs_off" },
	{ 0xd697e69a, "trace_hardirqs_on" },
	{ 0x4b0a3f52, "gic_nonsecure_priorities" },
	{ 0xb788fb30, "gic_pmr_sync" },
	{ 0x1e49e99f, "cpu_hwcaps" },
	{ 0x87882664, "gpiod_set_value" },
	{ 0xdcb764ad, "memset" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0x31909811, "cpu_hwcap_keys" },
	{ 0x14b89635, "arm64_const_caps_ready" },
	{ 0x6b2dc060, "dump_stack" },
	{ 0x92997ed8, "_printk" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "D30C54287136412DF89110F");
