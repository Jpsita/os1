INC=-I../include
CURR_DIR=$(shell pwd)

build: build/elfl.rnb symbols/elfloader.sym

symbols/elfloader.sym: build/elfloader.elf
	$(TARGET)-objcopy --only-keep-debug "build/elfloader.elf" "symbols/elfloader.sym"
build/elfl.rnb: build/elfloader.elf
	$(TARGET)-objcopy -O binary "build/elfloader.elf" "build/elfl.rnb" -g -S -R .eh_frame
build/elfloader.elf: build/main.o elfloader.ld build/elfFuncs.o
	$(TARGET)-ld -Telfloader.ld -o "build/elfloader.elf" -m elf_i386 --nmagic -nostdlib -q
build/main.o: main.c
	$(TARGET)-gcc -c main.c -m32 -mtune=i386 -o "build/main.o" -gdwarf $(INC)
build/elfFuncs.o: elfFuncs.c
	$(TARGET)-gcc -c elfFuncs.c -m32 -mtune=i386 -o "build/elfFuncs.o" -gdwarf $(INC)
configure:
	mkdir -p build
	mkdir -p symbols
clean:
	rm -rf build/*
	rm -rf symbols/*
