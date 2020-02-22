#!/bin/sh

target_module="grassdrv"
target_device="grassdrv"
perms=664 # User: Read (4), Write (2), no exec
          # Group: Read (4), Write (2), no exec
          # Other: Read (4), no write, no exec

# ----- Install the module -----
echo "Installing '$target_module' character device driver module."

insmod ./$target_module.ko || exit 1 # '$*' would pass arguments passed to this script

echo "Successfully installed $target_module module:"

lsmod | grep $target_module

echo ""

# ----- Create device's file node -----
# Our chardev module is initialized by (and into..) the kernel.
#
# We wrote the initialization code to have the kernel dynamically assign
# major and minor device numbers.
#
# The major number represents the character device driver module ID that will
# manage our devices. The listing of device driver modules and their major
# numbers can be found in /proc/devices.
#
# The minor number we get will be our index into our assigned device driver's
# list of responsibilities. (ie: each device is a <minor>'th chardev device assigned
# to the <major> character device driver module.
#
# So, now we can see why dynamically initializing our module's device numbers
# is a problem: We must create our module's device file node AFTER initializing
# the module, and then parsing our dynamically assigned major char device.
#
# In my simple example where the 'grassdrv' module manages one 'grassdrv' device:
# Major == Number kernel assigns to our 'grassdrv' characte device driver.
# Minor == 1 (Index of 'gdrv' device in the 'grassdrv' driver module).

echo "Generating $target_device device node"

# Find major number assigned to us
my_type="c" # character device driver
my_major=$(awk "\$2==\"$target_module\" {print \$1}" /proc/devices)
my_minor=0 # We only are managing one device, so use zero'th index.

echo "$my_major: Major number for $target_module device driver module"

echo "$my_minor: Minor number for $target_device device"

# Remove stale device node (..if it exists):
rm -f /dev/$target_device

# Add new character device node
mknod /dev/$target_device $my_type $my_major $my_minor

# Determine group for permission modification
if grep -q '^staff:' /etc/group; then
    group="staff"
else
    group="wheel"
fi

# Update permissions
# - Script was run as superuser, so need to open up access
chgrp $group /dev/$target_device
chmod $perms /dev/$target_device

# Print some evidence that we're all done.
echo "Finished: $my_type $my_major $my_minor"
ls -l /dev/$target_device
