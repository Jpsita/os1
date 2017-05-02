build: build/bootloader.bin symbols/bootloader.sym

build/bootloader.bin: build/bootloader.elf
	objcopy -O binary "build/bootloader.elf" "build/bootloader.bin"
symbols/bootloader.sym: build/bootloader.elf
	objcopy --only-keep-debug build/bootloader.elf  symbols/bootloader.sym
build/bootloader.o: entry.asm
	nasm -g -felf32 -F dwarf entry.asm -o "build/bootloader.o"
build/bootloader.elf: build/bootloader.o
	ld -Tloader.ld -m elf_i386 --nmagic -nostdlib -static -o "build/bootloader.elf" -Map symbols/loader.map
configure:
	mkdir -p build
	mkdir -p symbols
clean:
	rm -rf build/*
	rm -rf symbols/*
