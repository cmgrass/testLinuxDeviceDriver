#ifndef _GRASSDRV_H_ /* multiple include protection */
#define _GRASSDRV_H_

/* ----- prototypes ----- */
int grassdrv_open(struct inode *inode_p, struct file *file_p);

ssize_t grassdrv_read(struct file *file_p, char __user *buff_p, size_t count,
                      loff_t *f_pos_p);

ssize_t grassdrv_write(struct file *file_p, const char __user *buff_p,
                       size_t count, loff_t *f_pos_p);

int grassdrv_release(struct inode *inode_p, struct file *file_p);

/* ----- structures ----- */
/* TODO: Should we place device number structure in here too? */
struct grassdrv_dev_t {
  struct cdev cdev;
};

#endif /* _GRASSDRV_H_ */
