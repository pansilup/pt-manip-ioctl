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
	{ 0x4a022e20, "module_layout" },
	{ 0x4864da67, "misc_deregister" },
	{ 0x911df7c0, "misc_register" },
	{ 0xa92ec74, "boot_cpu_data" },
	{ 0x272cbe6c, "put_devmap_managed_page" },
	{ 0xdaee4ad7, "__put_page" },
	{ 0x587f22d7, "devmap_managed_key" },
	{ 0x33af6ba2, "get_user_pages_fast" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0xcd49ee62, "current_task" },
	{ 0xd0da656b, "__stack_chk_fail" },
	{ 0x97651e6c, "vmemmap_base" },
	{ 0x92997ed8, "_printk" },
	{ 0x95730ba, "alloc_pages" },
	{ 0x7cd8d75e, "page_offset_base" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");

