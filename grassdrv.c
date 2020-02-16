#include <linux/init.h>
#include <linux/module.h>

/* ----- prototypes ----- */
int grassdrv_open(struct inode *inode_p, struct file *file_p);
ssize_t grassdrv_read(struct file *file_p, char __user *buff_p, size_t count,
                      loff_t *fpos_p);
ssize_t grassdrv_write(struct file *file_p, const char *buff_p, size_t count,
                      loff_t *fpos_p);
int grassdrv_release(struct indoe *inode_p, struct file *file_p);

/* ----- file_operations structure -----
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
}

module_init(grassdrv_init);
module_exit(grassdrv_exit);
