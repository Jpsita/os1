#!/bin/bash

#Compiling files....
echo Compliling files...
nasm -g -felf32 entry.asm -o "build/entry.o"
nasm -g -felf32 utils.asm -o "build/utils.o"
gcc -c main.c -o "build/main.o" -m32
gcc -c -S main.c -o "build/main.list" -m32

#Linking files...
echo Linking files...
ld -Tlinker.ld -m elf_i386 --nmagic -nostdlib -static -o "build/bootload.elf"
objcopy -O binary "build/bootload.elf" bootload.out
objcopy --only-keep-debug "build/bootload.elf" bootload.sym
dd if=/dev/zero of=./bootload.bin bs=512 count=2880
dd if=./bootload.out of=./bootload.bin 
