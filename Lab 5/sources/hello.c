/* hello.c − Hello World kernel module
 *
 * Demonstrates module initialization, module release and printk.
 */
#include <linux/module.h>   /* Needed by all modules */
#include <linux/kernel.h>   /* Needed for KERN_* and printk */
#include <linux/init.h>     /* Needed for __init and __exit macros */

/* This function is run upon module load. This is where you setup data
* structures and reserve resources used by the module. */
static int __init my_init(void)
{
    /* Linux kernel's version of printf */
    printk(KERN_INFO "Hello world!\n");

    // A non 0 return means init_module failed; module can't be loaded.
    return 0;
}

/* This function is run just prior to the module's removal from the
system. You should release _ALL_ resources used by your module
here (otherwise be prepared for a reboot). */
static void __exit my_exit(void)
{
    printk(KERN_ALERT "Goodbye world!\n");
}

/* These define information that can be displayed by modinfo */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ECEN749 Student");
MODULE_DESCRIPTION("Simple Hello World Module");

/* Define functions for initialization and cleanup */
module_init(my_init);
module_exit(my_exit);

