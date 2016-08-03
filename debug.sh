#!/bin/bash
screen -S emulator -d -m qemu-system-i386 -fda bootload.bin -S -s -m 512M
gdb  -command=gdbScript 
