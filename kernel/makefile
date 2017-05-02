INC=-I../include/kernel

build: build/kernel.rnb symbols/kernel.sym

build/kernel.rnb: build/kernel.elf
	objcopy -O binary "build/kernel.elf" "build/kernel.rnb" -g -S -R .eh_frame
symbols/kernel.sym: build/kernel.elf
	objcopy --only-keep-debug "build/kernel.elf" "symbols/kernel.sym"
build/kernel.elf: build/utils.o build/main.o build/video.o build/keyboard.o build/string.o build/fat.o build/interrupt.o build/floppy.o build/rtc.o build/scancodes.o kernel.ld
	ld -Tkernel.ld -m elf_i386 --nmagic -nostdlib -static -o "build/kernel.elf" -Map symbols/kernel.map
build/utils.o : utils.asm
	nasm -g -felf32 -F dwarf utils.asm -o "build/utils.o"
build/main.o : main.c ../include/kernel/keyboard.h ../include/kernel/video.h ../include/kernel/utils.h ../include/kernel/interrupt.h ../include/kernel/floppy.h ../include/kernel/rtc.h
	gcc -c main.c -o "build/main.o" -m32 -mtune=i386 -gdwarf $(INC)
build/video.o : video.c ../include/kernel/main.h ../include/kernel/video.h
	gcc -c video.c -o "build/video.o" -m32 -mtune=i386 -gdwarf $(INC)
build/keyboard.o : keyboard.c ../include/kernel/keyboard.h ../include/kernel/scancodes.h ../include/kernel/video.h ../include/kernel/utils.h ../include/kernel/interrupt.h
	gcc -c keyboard.c -o "build/keyboard.o" -m32 -mtune=i386 -gdwarf $(INC)
build/interrupt.o : interrupt.c ../include/kernel/interrupt.h ../include/kernel/keyboard.h ../include/kernel/utils.h ../include/kernel/video.h
	gcc -c interrupt.c -o "build/interrupt.o" -m32 -mtune=i386 -gdwarf $(INC)
build/floppy.o : floppy.c ../include/kernel/floppy.h ../include/kernel/utils.h ../include/kernel/video.h ../include/kernel/interrupt.h ../include/kernel/rtc.h
	gcc -c floppy.c -o "build/floppy.o" -m32 -mtune=i386 -gdwarf $(INC)
build/rtc.o : rtc.c ../include/kernel/rtc.h ../include/kernel/utils.h ../include/kernel/interrupt.h 
	gcc -c rtc.c -o "build/rtc.o" -m32 -mtune=i386 -gdwarf $(INC)
build/scancodes.o : scancodes.c ../include/kernel/scancodes.h 
	gcc -c scancodes.c -o "build/scancodes.o" -m32 -mtune=i386 -DKEYB_TYPE_CH -gdwarf $(INC)
build/fat.o: fat.c ../include/kernel/fat.h ../include/kernel/string.h
	gcc -c fat.c -o "build/fat.o" -m32 -mtune=i386 -gdwarf $(INC)
build/string.o: string.c ../include/kernel/string.h
	gcc -c string.c -o "build/string.o" -m32 -mtune=i386 -gdwarf $(INC)
configure: 
	mkdir -p build
	mkdir -p symbols
clean:
	rm -rf build/*
	rm -rf symbols/*

	
