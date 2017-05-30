INC=-I../include
CURR_DIR=$(shell pwd)

build: build/elfloader.rnb symbols/elfloader.sym

symbols/elfloader.sym: build/elfloader.elf
	objcopy --only-keep-debug "build/elfloader.elf" "symbols/elfloader.sym"
build/elfloader.rnb: build/elfloader.elf
	objcopy -O binary "build/elfloader.elf" "build/elfloader.rnb" -g -S -R .eh_frame
build/elfloader.elf: build/main.o elfloader.ld
	ld -Telfloader.ld -o "build/elfloader.elf" -m elf_i386 --nmagic -nostdlib -q
build/main.o: main.c
	gcc -c main.c -m32 -mtune=i386 -o "build/main.o" -gdwarf $(INC)
configure:
	mkdir -p build
	mkdir -p symbols
clean:
	rm -rf build/*
	rm -rf symbols/*
