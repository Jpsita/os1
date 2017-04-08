#!/bin/bash
parent_path=$( cd "$(dirname "${BASH_SOURCE}")" ; pwd -P )
cd "$parent_path"


echo Running...
tmux new -s qemu -d "qemu-system-i386 -fda images/floppy.img -m 512M"

