#!/bin/bash
parent_path=$( cd "$(dirname "${BASH_SOURCE}")" ; pwd -P )
cd "$parent_path"


echo Running...
tmux new -s qemu -d "qemu-system-i386 -fda bootload.bin -m 512M" 

