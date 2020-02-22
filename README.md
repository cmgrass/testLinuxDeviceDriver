## These are my experiments stemming from:
https://lwn.net/Kernel/LDD3/

## Maintained in recent kernel version by the following project:
https://github.com/martinezjavier/ldd3/

## ----- Notes -----
The makefile is assuming that the currently running kernel's source tree
is where the kernel was built, or at least where it could be built.

The makefile uses the following command to verify the running kernel's source
tree is a valid directory:
$ uname -r
(I can get the full system information by: $ uname -a)

The makefile then passes this project's directory to the kernel's source
tree for building the module.

## To make:
In the root directoy, just run:
```
$ make
```

## To install device driver module:
Install script
```
$ sudo sh install_gdrv.sh
```

Otherwise to manually install, the script is doing at least the following:
Use insmod
```
$ sudo insmod grassdrv.ko
                    ^ Kernel object
```

## To verify it was installed:
```
$ lsmod | grep grassdrv
grassdrv                  16384  0
```

## Our debug is printing to the syslog.
## (because use printk for kernel modules, and for the experiment using KERN_ALERT)
```
$ tail /var/log/syslog
.
.
Feb 16 13:24:24 cmgrass-ubuntu kernel: [ 1603.534526] grassdrv: loading out-of-tree module taints kernel.
Feb 16 13:24:24 cmgrass-ubuntu kernel: [ 1603.534595] grassdrv: module verification failed: signature and/or required key missing - tainting kernel
Feb 16 13:24:24 cmgrass-ubuntu kernel: [ 1603.535161] HELLO World!
.
.
```

Note: You can pass  '-f' to 'tail' so it follows the written stream

## Remove the module
Uninstall script
```
$ sudo sh uninstall_gdrv.sh
```

Otherwise, the script is doing at least the following:
```
$ sudo rmmod grassdrv
$ lsmod | grep grassdrv
$ tail /var/log/syslog
.
.
Feb 16 13:29:30 cmgrass-ubuntu kernel: [ 1909.263426] Goodbye, cruel world
.
.
```

## Block diagram
This diagram contains my view of the key structures and interfaces.

```
Three Key Structures:
1) struct file_operations (fops)
   The fops is a global file operations structure that contains:
   - Pointer to module that owns it
   - Pointer to functions that handle file operations (open, read, write, etc..)

   Any time a file is opened, the kernel will assign a pointer to the associated
   driver's fops structure.

   If a user program opens a file, we can think of the file as an 'object',
   and the callbacks in the fops structure are the 'methods' that act on the file.

2) struct file
   Not to be confused with user space C library FILE structure, this kernel space
   file structure is allocated by the kernel whenever open is called on a file.
   The kernel will point the file structure's '*f_op' member to the target
   device's file_operations structure.

   There are other important fields, but these are the basics.

3) inode
   An 'inode' structure is used by the kernel internnaly to represent files.
   Multiple file structures can exist for the same file, but they all point
   to only one inode for the file.

   For device drivers, this contains two key data structures:
   - Major and Minor device number.
   - Pointer to char device structure.
     (because in our case, we registered a chardev)



/---------------------/  /---------------------/  /---------------------/
/      User app A     /  /      User app A     /  /      User app A     /
/ open(/dev/grassdrv) /  / open(/dev/grassdrv) /  / open(/dev/grassdrv) /
/---------------------/  /---------------------/  /---------------------/
/     struct FILE     /  /     struct FILE     /  /     struct FILE     /
/---------------------/  /---------------------/  /---------------------/
                     \              |              /
                      \             |             /
                       \            |            /
                        \       user space      /
--------------------------------------------------------------------------------
                          \    kernel space   /
                           \        |        /
                            \       |       /
                           /-----------------/
  _______________ _________/      inode      /________
 |               |         /-----------------/        |
 |               |                  |                 |
 |               |                  |                 |
 |        /--------------/  /--------------/  /--------------/
 |        / struct file  /  / struct file  /  / struct file  /
 |        / (instance A) /  / (instance A) /  / (instance A) /
 |        /--------------/  /--------------/  /--------------/
 |            / ____________________/                /
 |           / / ___________________________________/
 |          / / /
 |  /----------------------------------------------------------/
 |  /      | | |    Device Driver Module                       /
 |  /      | | |                                               /
 |  / /-------------------------/  /-------------------------/ /
 |  / / struct file_operations  /  / struct file_operations  / /
 |  / /-------------------------/  /-------------------------/ /
  --/-/-->  char dev struct     /  /      char dev struct    / /
    / /-------------------------/  /-------------------------/ /
    / /     Device A driver     /  /     Device Z driver     / /
    / /    (grassdrv) driver    /  / (n/a, I only have one   / /
    / /                         /  /  device driver)         / /
    / /-------------------------/  /-------------------------/ /
    /                                                          /
    /----------------------------------------------------------/

```
