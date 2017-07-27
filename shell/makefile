INC=-I../include
CURR_DIR=$(shell pwd)

build: build/shell.rnb symbols/shell.sym

symbols/shell.sym: build/shell.elf
	$(TARGET)-objcopy --only-keep-debug "build/shell.elf" "symbols/shell.sym"
build/shell.rnb: build/shell.elf
	$(TARGET)-objcopy -O binary "build/shell.elf" "build/shell.rnb" -g -S -R .eh_frame
build/shell.elf: build/shell.o shell.ld
	$(TARGET)-ld -Tshell.ld -o "build/shell.elf" -m elf_i386 --nmagic -nostdlib -q
build/shell.o: shell.c
	$(TARGET)-gcc -c shell.c -m32 -mtune=i386 -o "build/shell.o" -gdwarf $(INC) $(MY_CFLAGS)
configure:
	mkdir -p build
	mkdir -p symbols
clean:
	rm -rf build/*
	rm -rf symbols/*