#!/bin/bash

#Compiling files....
echo Compliling files...
nasm -g -felf32 -F dwarf entry.asm -o "build/entry.o"
nasm -g -felf32 -F dwarf utils.asm -o "build/utils.o"
gcc -c main.c -o "build/main.o" -m32 -mtune=i386 -gdwarf
gcc -c -S main.c -o "build/main.list" -m32 -mtune=i386 -gdwarf
gcc -c keyboard.c -o "build/keyboard.o" -m32 -mtune=i386 -gdwarf
gcc -c video.c -o "build/video.o" -m32 -mtune=i386 -gdwarf
gcc -c -S video.c -o "build/video.list" -m32 -mtune=i386 -gdwarf
gcc -c interrupt.c -o "build/interrupt.o" -m32 -mtune=i386 -gdwarf

#Linking files...
echo Linking files...
ld -Tlinker.ld -m elf_i386 --nmagic -nostdlib -static -o "build/bootload.elf" -M -g > "build/map.txt"
objcopy -O binary "build/bootload.elf" "build/bootload.out"
objcopy --only-keep-debug "build/bootload.elf" "bootload.sym"
dd if=/dev/zero of=./bootload.bin bs=512 count=2880
dd if=./build/bootload.out of=./bootload.bin
