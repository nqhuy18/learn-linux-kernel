#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

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
	{ 0x65487097, "__x86_indirect_thunk_rax" },
	{ 0x1bf79915, "eth_type_trans" },
	{ 0x230caca3, "netif_rx" },
	{ 0x55d225b, "rtnl_link_register" },
	{ 0xc7a4fbed, "rtnl_lock" },
	{ 0xc07351b3, "__SCT__cond_resched" },
	{ 0x24ad6361, "alloc_netdev_mqs" },
	{ 0xb9b3d177, "register_netdevice" },
	{ 0xbed2fe94, "free_netdev" },
	{ 0x6e720ff2, "rtnl_unlock" },
	{ 0x47f26ecb, "ether_setup" },
	{ 0x41ed3709, "get_random_bytes" },
	{ 0x32a68c03, "dev_addr_mod" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x479b90bb, "param_ops_int" },
	{ 0x40538405, "ethtool_op_get_ts_info" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x122c3a7e, "_printk" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x96994818, "rtnl_link_unregister" },
	{ 0x53569707, "this_cpu_off" },
	{ 0xe2fd41e5, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "4455C850B5C8ACC9D4392D9");
