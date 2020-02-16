# ----- Source -----
## These are my experiments stemming from:
https://lwn.net/Kernel/LDD3/

## Maintained in recent kernel version by the following project:
https://github.com/martinezjavier/ldd3/blob/master/

# ----- Notes -----
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
$ make

## To install device driver module:
Use insmod
$ insmod [filename] [module options...]

EX: (sudo)
$ sudo insmod hello.ko
                    ^ Kernel object

## To verify it was installed:
$ lsmod | grep hello
hello                  16384  0

## Our debug is printing to the syslog.
## (bcause use printk for kernel modules, and for the experiment using KERN_ALERT)
$ tail /var/log/syslog
.
.
Feb 16 13:24:24 cmgrass-ubuntu kernel: [ 1603.534526] hello: loading out-of-tree module taints kernel.
Feb 16 13:24:24 cmgrass-ubuntu kernel: [ 1603.534595] hello: module verification failed: signature and/or required key missing - tainting kernel
Feb 16 13:24:24 cmgrass-ubuntu kernel: [ 1603.535161] HELLO World!
.
.

## Remove the module
$ sudo rmmod hello
$ lsmod | grep hello
$ tail /var/log/syslog
.
.
Feb 16 13:29:30 cmgrass-ubuntu kernel: [ 1909.263426] Goodbye, cruel world
.
.
