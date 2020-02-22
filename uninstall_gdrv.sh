#!/bin/sh

target_module="grassdrv"

echo "Uninstalling $target_module"

rmmod $target_module

echo "Successfully uninstalled $target_module"
