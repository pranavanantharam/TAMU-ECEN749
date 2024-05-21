#include <linux/module.h>  /* Needed by all modules */
#include <linux/moduleparam.h>  /* Needed for module parameters */
#include <linux/kernel.h>  /* Needed for printk and KERN_* */
#include <linux/init.h>	   /* Need for __init macros  */
#include <linux/fs.h>	   /* Provides file ops structure */
#include <linux/sched.h>   /* Provides access to the "current" process task structure */
#include <asm/uaccess.h>   /* Provides utilities to bring user space data into kernel space.  Note, it is processor arch specific. */
#include <linux/slab.h>	   /* kmalloc and kfree definitions */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/io.h>
#include "xparameters.h"
#include <linux/ioport.h>   // IO memory allocation

// From xparameters.h
#define PHY_ADDR	XPAR_MULTIPLY_0_S00_AXI_BASEADDR // physical address of multiplier

// Size of physical address range for multiply
#define MEMSIZE		XPAR_MULTIPLY_0_S00_AXI_HIGHADDR - XPAR_MULTIPLY_0_S00_AXI_BASEADDR + 1

/* Some defines */
#define DEVICE_NAME	"multiplier"
#define BUF_LEN		80

/* Function prototypes, so we can setup the function pointers for dev
   file access correctly. */
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_close(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

// Virtual address pointing to multiplier
void* virt_addr;

/* 
 * Global variables are declared as static, so are global but only
 * accessible within the file.
 */
static int Major;		/* Major number assigned to our device driver */
static int Device_Open = 0;	/* Flag to signify open device */


/* This structure defines the function pointers to our functions for
   opening, closing, reading and writing the device file.  There are
   lots of other pointers in this structure which we are not using,
   see the whole definition in linux/fs.h */
static struct file_operations fops = {
  .read = device_read,
  .write = device_write,
  .open = device_open,
  .release = device_close
};


static int __init my_init(void)
{

  printk(KERN_INFO "Mapping virtual address... \n");
	
  // map virtual address to multiplier physical address
  virt_addr = ioremap(PHY_ADDR, MEMSIZE);

  printk(KERN_INFO "Physical Address = %p \n", PHY_ADDR);
  printk(KERN_INFO "Virtual Address = %p \n", virt_addr);
  
  /* This function call registers a device and returns a major number
     associated with it.  Be wary, the device file could be accessed
     as soon as you register it, make sure anything you need (ie
     buffers ect) are setup _BEFORE_ you register the device.*/
  Major = register_chrdev(0, DEVICE_NAME, &fops);

  
  /* Negative values indicate a problem */
  if (Major < 0) {		
    /* Make sure you release any other resources you've already
       grabbed if you get here so you don't leave the kernel in a
       broken state. */
    printk(KERN_ALERT "Registering char device failed with %d\n", Major);
    return Major;
  }

  printk(KERN_INFO "Registered a device with dynamic Major number of %d\n", Major);
  printk(KERN_INFO "Create a device file for this device with this command:\n'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);

  return 0;		/* success */
}


static void __exit my_exit(void)
{
	// Unregister the device
	unregister_chrdev(Major, DEVICE_NAME);

	printk(KERN_ALERT "unmapping virtual address space... \n");
	iounmap((void *)virt_addr);
}


/* 
 * Called when a process tries to open the device file, like "cat
 * /dev/my_chardev".  Link to this function placed in file operations
 * structure for our device file.
 */
static int device_open(struct inode *inode, struct file *file)
{  


/* In these case we are only allowing one process to hold the device
     file open at a time. */
  if (Device_Open)		/* Device_Open is my flag for the
				   usage of the device file (definied
				   in my_chardev.h)  */
    return -EBUSY;		/* Failure to open device is given
				   back to the userland program. */

  Device_Open++;		/* Keeping the count of the device
				   opens. */	

  
	try_module_get(THIS_MODULE);	/* increment the module use count
				   (make sure this is accurate or you
				   won't be able to remove the module
				   later. */

	printk("Device opened successfully \n");
	return 0;
}


/* 
 * Called when a process closes the device file.
 */
static int device_close(struct inode *inode, struct file *file)
{

	Device_Open--;		/* We're now ready for our next
				   caller */
  
  /* 
   * Decrement the usage count, or else once you opened the file,
   * you'll never get get rid of the module.
   */
	module_put(THIS_MODULE);
  
	printk("Device closed succesfully \n");
	return 0;
}


/* 
 * Called when a process, which already opened the dev file, attempts to
 * read from it.
 */
static ssize_t device_read(struct file *filp,	/* see include/linux/fs.h   */
			   char *buffer,	/* buffer to fill with data */
			   size_t length,	/* length of the buffer     */
			   loff_t * offset)
{
  /*
   * Number of bytes actually written to the buffer 
   */
  int bytes_read = 0;
  
  // Allocating kernel buffer
  int * kernel_buffer = (int*)kmalloc(length * sizeof(int), GFP_KERNEL );
  
  // Read values from registers into kernel buffer
  kernel_buffer[0] = ioread32(virt_addr);
  kernel_buffer[1] = ioread32(virt_addr+4);
  kernel_buffer[2] = ioread32(virt_addr+8);
  
  printk("Kernel Buffer[0] = %d \n", kernel_buffer[0]);
  printk("Kernel Buffer[1] = %d \n", kernel_buffer[1]);
  printk("Kernel Buffer[2] = %d \n", kernel_buffer[2]);
  
  // Using char pointer
  char * kbuf = (char*)kernel_buffer;
  int i ;
  for( i = 0; i < length; i++ )
  {
	// Write / copy values from kernel space to user space buffer
	put_user(*(kbuf++), buffer++); // one char at a time...
	bytes_read++;
  }
 
  
  kfree(kernel_buffer);
  
  /* 
   * Most read functions return the number of bytes put into the buffer
   */
  return bytes_read;
}


/*  
 * Called when a process writes to dev file: echo "hi" > /dev/hello
 * Next time we'll make this one do something interesting.
 */
static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
  int i;
  
  char * kernel_buffer = (char*)kmalloc((len+1)*sizeof(char), GFP_KERNEL);
  
  for( i = 0; i < len; i++ )
  {
	  // Get / copy values from user space into kernel buffer
	  get_user(kernel_buffer[i], buff+i);
  }
  
  kernel_buffer[len] = '\0';
  
  // Using int pointer
  int * int_buf = (int*)kernel_buffer;
  
  printk(KERN_INFO, "Writing %d to register 0 \n", int_buf[0]);
  iowrite32(int_buf[0], virt_addr+0);
  
  printk(KERN_INFO, "Writing %d to register 0 \n", int_buf[1]);
  iowrite32(int_buf[1], virt_addr+4);
  
  kfree(int_buf);
  
  /* 
   * Again, return the number of input characters used 
   */
  return i;
}


/* Display information that can be displayed by modinfo */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pranav Anantharam");
MODULE_DESCRIPTION("Multiplier Character Device Driver");

/* Functions to use for initialization and cleanup */
module_init(my_init);
module_exit(my_exit);
