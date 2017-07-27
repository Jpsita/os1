INC=-I../include
CURR_DIR=$(shell pwd)

build: build/test.elf symbols/test.sym

symbols/test.sym:
	$(TARGET)-objcopy --only-keep-debug "build/test.elf" "symbols/test.sym" 
build/test.elf: build/test.o
	$(TARGET)-ld -Ttest.ld -o "build/test.elf" build/test.o -m elf_i386 --nmagic -nostdlib -q -static ../libKernel/build/libkernel.o ../libKernel/build/utils.o -e main
build/test.o: test.c
	$(TARGET)-gcc -c test.c -m32 -mtune=i386 -o "build/test.o" -gdwarf $(INC)
configure:
	mkdir -p build
	mkdir -p symbols
clean:
	rm -rf build/*
	rm -rf symbols/*
