#!/bin/sh

target_module="grassdrv"
target_device="grassdrv"

echo "Uninstalling $target_module char device driver module"

rmmod $target_module || exit 1

echo "Successfully uninstalled $target_module module"

echo ""

echo "Removing $target_device device file node"

rm -f /dev/$target_device

echo "Finished."
