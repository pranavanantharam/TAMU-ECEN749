#include <linux/module.h>   // Needed by all modules
#include <linux/kernel.h>   // Needed for KERN_* and printk
#include <linux/init.h>     // Needed for __init and __exit macros
#include <asm/io.h>         // Needed for IO reads and writes
#include "xparameters.h"    // Needed for IO reads and writes
#include <linux/ioport.h>   // IO memory allocation

// From xparameters.h
#define PHY_ADDR    XPAR_MULTIPLY_0_S00_AXI_BASEADDR // physical address of multiplier

// Size of physical address range for multiply
#define MEMSIZE     XPAR_MULTIPLY_0_S00_AXI_HIGHADDR - XPAR_MULTIPLY_0_S00_AXI_BASEADDR + 1

// Virtual address pointing to multiplier
void *virt_addr;

/* This function is run upon module load. This is where you setup data
structures and reserve resources used by the module */
static int __init my_init(void)
{
    printk(KERN_INFO "Mapping virtual address... \n");

    // map virtual address to multiplier physical address
    // use ioremap
    virt_addr = ioremap(PHY_ADDR, MEMSIZE);

    // print the physical and virtual address
    printk(KERN_INFO "Physical Address = %p \n", PHY_ADDR);
    printt(KERN_INFO "Virtual Address = %p \n", virt_addr);

    // Write register 7 to register 0
    printk(KERN_INFO "Writing a 7 to register 0 \n");
    // use iowrite32
    iowrite32(7, virt_addr + 0); // base address + offset

    // Write 2 to register 1
    printk(KERN_INFO "Writing a 2 to register 1 \n");
    // use iowrite32
    iowrite32(2, virt_addr+4);

    printk("Read %d from register 0 \n", ioread32(virt_addr + 0) );
    printk("Read %d from register 1 \n", ioread32(virt_addr + 4) );
    printk("Read %d from register 2 \n", ioread32(virt_addr + 8) );

    // A non 0 return means init_module failed; module can't be loaded
    return 0;
}

/* This function is run just prior to the module's removal from the system.
You should release ALL resources used by your module here (otherwise be
prepared for a reboot). */
static void __exit my_exit(void)
{
    printk(KERN_ALERT "unmapping virtual address space... \n");
    iounmap((void *)virt_addr);
}

/* Display information that can be displayed by modinfo */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ECEN749 Student");
MODULE_DESCRIPTION("Simple multiplier module");

/* Functions to use for initialization and cleanup */
module_init(my_init);
module_exit(my_exit);
