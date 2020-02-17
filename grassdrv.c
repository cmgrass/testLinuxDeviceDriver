#include <linux/init.h>
#include <linux/module.h>

#include <linux/kernel.h>	/* printk() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */
#include <linux/cdev.h>

#include "grassdrv.h"

#define SUCCESS 0
#define GRASSDRV_MAX_DEVICES 1
#define GRASSDRV_DEV_MINOR_START 0
#define GRASSDRV_DEV_NAME_STR "gdrv"

/* ----- globals ----- */
/* Globals for module parameters */
int grassdrv_dev_major = 0;
int grassdrv_dev_minor = 0;
int grassdrv_count = GRASSDRV_MAX_DEVICES; /* TODO: FYI, the code is written for only ONE device */

/* Globals for use with the module code */
struct grassdrv_dev_t grassdrv_device;

/* file_operations structure
 * Notes:
 * - One fops structure is allocated for each open file.
 *
 * - The fops structure generally contains function pointers to the driver
 *   functions that implement functionality for associated system call.
 *   The exception is the '.owner' member, which is a pointer to the device
 *   module owning the structure. This prevents a module from unloading if
 *   it has open files.
 *
 * - This is an object oriented programming concept, with the file being the
 *   'object', and the functions operating on it being the 'methods'.
 *
 * - Obviously, my implementation below is only a sampling of available
 *   system calls.
 */
struct file_operations grassdrv_fops = {
  .owner = THIS_MODULE,        /* Macro is almost always used. See note above. */
  .open =  grassdrv_open,
  .read =  grassdrv_read,
  .write = grassdrv_write,
  .release = grassdrv_release, /* TODO: Is this needed? (Is it 'close()'?) */
};


/* ----- module parameters ----- */
MODULE_AUTHOR("Christopher M. Grass");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module to help me understand device driver modules.");
module_param(grassdrv_dev_major, int, S_IRUGO);
module_param(grassdrv_dev_minor, int, S_IRUGO);
module_param(grassdrv_count, int, S_IRUGO);


/* ----- functions ----- */
static int grassdrv_init(void)
{
  int                   status;
  dev_t                 devnums;

  /* 1) Get dynamically-allocated driver numbers from the system
   * Major: Represent the driver that will manage the device
   * Minor: Position in that driver's list
   */
  status = alloc_chrdev_region(&devnums, GRASSDRV_DEV_MINOR_START,
                               grassdrv_count, GRASSDRV_DEV_NAME_STR);
  if (status < SUCCESS) {
    printk(KERN_ALERT "[grassdrv]_init: Could not allocate cdev numbers:%#x\n",
           status);
    return status;
  }

  grassdrv_dev_major = MAJOR(devnums);
  grassdrv_dev_minor = MINOR(devnums);

  /* 2) Setup cdev structure
   * These are kernel structures used to represent char devices internally
   */
  memset(&grassdrv_device, 0, sizeof(struct grassdrv_dev_t));

  cdev_init(&(grassdrv_device.cdev), &grassdrv_fops);
  grassdrv_device.cdev.owner = THIS_MODULE;
  grassdrv_device.cdev.ops = &grassdrv_fops;
  status = cdev_add(&(grassdrv_device.cdev), devnums, grassdrv_count);

  if (status != SUCCESS) {
    printk(KERN_ALERT "[grassdrv]_init: Could not add this module to the kernel %#x\n",
           status);
    return status;
  }

  printk(KERN_ALERT "grassdrv device driver module installed\n");
  return SUCCESS;
}

static void grassdrv_exit(void)
{
  dev_t devnums;

  devnums = MKDEV(grassdrv_dev_major, grassdrv_dev_minor);

  printk(KERN_ALERT "[grassdrv]_exit: removing module\n");

  cdev_del(&(grassdrv_device.cdev));

  unregister_chrdev_region(devnums, grassdrv_count);

  printk(KERN_ALERT "grassdrv module removed\n");
}

module_init(grassdrv_init);
module_exit(grassdrv_exit);
