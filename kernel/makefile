build: build/kernel.rnb symbols/kernel.sym

build/kernel.rnb: build/kernel.elf
	objcopy -O binary "build/kernel.elf" "build/kernel.rnb" -g -S -R .eh_frame
symbols/kernel.sym: build/kernel.elf
	objcopy --only-keep-debug "build/kernel.elf" "symbols/kernel.sym"
build/kernel.elf: build/utils.o build/main.o build/video.o build/keyboard.o build/string.o build/fat.o build/interrupt.o build/floppy.o build/rtc.o build/scancodes.o kernel.ld
	ld -Tkernel.ld -m elf_i386 --nmagic -nostdlib -static -o "build/kernel.elf"
build/utils.o : utils.asm
	nasm -g -felf32 -F dwarf utils.asm -o "build/utils.o"
build/main.o : main.c keyboard.h video.h utils.h interrupt.h floppy.h rtc.h
	gcc -c main.c -o "build/main.o" -m32 -mtune=i386 -gdwarf
build/video.o : video.c main.h video.h
	gcc -c video.c -o "build/video.o" -m32 -mtune=i386 -gdwarf
build/keyboard.o : keyboard.c keyboard.h scancodes.h video.h utils.h interrupt.h
	gcc -c keyboard.c -o "build/keyboard.o" -m32 -mtune=i386 -gdwarf
build/interrupt.o : interrupt.c interrupt.h keyboard.h utils.h video.h
	gcc -c interrupt.c -o "build/interrupt.o" -m32 -mtune=i386 -gdwarf
build/floppy.o : floppy.c floppy.h utils.h video.h interrupt.h rtc.h
	gcc -c floppy.c -o "build/floppy.o" -m32 -mtune=i386 -gdwarf
build/rtc.o : rtc.c rtc.h utils.h interrupt.h
	gcc -c rtc.c -o "build/rtc.o" -m32 -mtune=i386 -gdwarf
build/scancodes.o : scancodes.c scancodes.h
	gcc -c scancodes.c -o "build/scancodes.o" -m32 -mtune=i386 -DKEYB_TYPE_CH -gdwarf
build/fat.o: fat.c fat.h
	gcc -c fat.c -o "build/fat.o" -m32 -mtune=i386 -gdwarf
build/string.o: string.c string.h
	gcc -c string.c -o "build/string.o" -m32 -mtune=i386 -gdwarf
configure: 
	mkdir build
	mkdir symbols
clean:
	rm -rf build/*
	rm -rf build/*

	