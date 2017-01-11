#!/bin/bash
parent_path=$( cd "$(dirname "${BASH_SOURCE}")" ; pwd -P )
cd "$parent_path"

tmux new -s scite -d "scite linker.ld entry.asm main.c video.h keyboard.h"
