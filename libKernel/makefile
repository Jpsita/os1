INC=$(ROOT_DIR)/include

build: build/libkernel.a

build/libkernel.a: build/libkernel.o build/utils.o
	ld --static build/libkernel.o build/utils.o -o "build/libkernel.a" -m elf_i386 --nmagic -nostdlib
build/libkernel.o: libkernel.c $(INC)/libkernel.h
	gcc -c libkernel.c -I $(INC) -o build/libkernel.o -m32 -mtune=i386 -gdwarf
build/utils.o: utils.asm $(INC)/libkernel/utils.h
	nasm -g -felf32 -F dwarf utils.asm -o build/utils.o
configure:
	mkdir -p build
	mkdir -p symbols
clean:
	rm -rf build/*
	rm -rf symbols/*