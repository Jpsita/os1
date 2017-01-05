#!/bin/bash

echo Running...
tmux new -s qemu -d "qemu-system-i386 -fda bootload.bin -m 512M" 

