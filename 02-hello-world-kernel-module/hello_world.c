#include <linux/module.h> // Required for all kernel modules
#include <linux/kernel.h> // For KERN_INFO
#include <linux/init.h> // For __init and __exit macros

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NQH");
MODULE_DESCRIPTION("A Simple Hello World Kernel Module");
MODULE_VERSION("0.01");

static int __init hello_init(void)
{
printk(KERN_INFO "Hello, World! Kernel Module Loaded.\n");
return 0; // Return 0 for successful loading
}

static void __exit hello_exit(void)
{
printk(KERN_INFO "Goodbye, World! Kernel Module Unloaded.\n");
}

module_init(hello_init);
module_exit(hello_exit);