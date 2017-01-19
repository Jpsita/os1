#!/bin/bash
#Compiling files....

ML="CH";

while [[ $# -gt 0 ]]
do
key="$1"


case $key in
	-IT)
	ML="IT"
	shift
	;;
	-CH)
	ML="CH"
	shift
	;;
	*)
	;;
esac

done

parent_path=$( cd "$(dirname "${BASH_SOURCE}")" ; pwd -P )
cd "$parent_path"

echo - Compliling files...
nasm -g -felf32 -F dwarf entry.asm -o "build/entry.o"
nasm -g -felf32 -F dwarf utils.asm -o "build/utils.o"
gcc -c main.c -o "build/main.o" -m32 -mtune=i386 -gdwarf
gcc -c -S main.c -o "build/main.list" -m32 -mtune=i386 -gdwarf
gcc -c keyboard.c -o "build/keyboard.o" -m32 -mtune=i386 -gdwarf
gcc -c -S keyboard.c -o "build/keyboard.list" -m32 -mtune=i386 -gdwarf
gcc -c video.c -o "build/video.o" -m32 -mtune=i386 -gdwarf
gcc -c -S video.c -o "build/video.list" -m32 -mtune=i386 -gdwarf
gcc -c interrupt.c -o "build/interrupt.o" -m32 -mtune=i386 -gdwarf
gcc -c floppy.c -o "build/floppy.o" -m32 -mtune=i386 -gdwarf
if [ "${ML}" == "CH" ]
then
	echo - Using CH Keyboard Layout
	gcc -c scancodes.c -o "build/scancodes.o" -m32 -mtune=i386 -gdwarf -DKEYB_TYPE_CH
else
	echo - Using IT keyboard Layout
	gcc -c scancodes.c -o "build/scancodes.o" -m32 -mtune=i386 -gdwarf -DKEYB_TYPE_IT
fi
#Linking files...
echo - Linking files...
ld -Tlinker.ld -m elf_i386 --nmagic -nostdlib -static -o "build/bootload.elf" -M -g > "build/map.txt"
#Generating Binary Output...
echo - Genereating Binary Output...
objcopy -O binary "build/bootload.elf" "build/bootload.out"
#Generating Debug Symbols File....
echo - Generating Debug Symbols File...
objcopy --only-keep-debug "build/bootload.elf" "bootload.sym"
#Generating System Floppy Image
echo - Generating System Floppy Image...
dd if=/dev/zero of=./bootload.bin bs=512 count=2880
dd if=./build/bootload.out of=./bootload.bin
#Done.
echo - Done.
