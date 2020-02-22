#!/bin/sh

target_module="grassdrv"

echo "Installing $target_module"

insmod ./$target_module.ko || exit 1 # '$*' would pass arguments passed to this script

echo "Successfully installed $target_module:"

lsmod | grep $target_module
