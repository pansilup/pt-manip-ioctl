#include <linux/init.h>
#include <linux/module.h>

static int __init mod_init(void) {

    printk(KERN_INFO "kernel_agent init!\n");
    return 0;
}

static void __exit mod_exit(void) {
    
    printk(KERN_INFO "exiting kernel_agent!\n");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("pansilup");
MODULE_DESCRIPTION("Change process PT mappings");
MODULE_VERSION("0.01");

