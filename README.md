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
