#!/bin/bash
parent_path=$( cd "$(dirname "${BASH_SOURCE}")" ; pwd -P )
cd "$parent_path"


tmux new -s emulator -d "qemu-system-i386 -fda bootload.bin -S -s -m 512M"
sleep 2
gdb  -command=gdbScript 
