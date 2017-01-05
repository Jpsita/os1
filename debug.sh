#!/bin/bash
tmux new -s emulator -d "qemu-system-i386 -fda bootload.bin -S -s -m 512M"
gdb  -command=gdbScript 
