INC=-I../include/kernel

build: build/kernel.rnb symbols/kernel.sym

build/kernel.rnb: build/kernel.elf
	$(TARGET)-objcopy -O binary "build/kernel.elf" "build/kernel.rnb" -g -S -R .eh_frame
symbols/kernel.sym: build/kernel.elf
	$(TARGET)-objcopy --only-keep-debug "build/kernel.elf" "symbols/kernel.sym"
build/kernel.elf: build/utils.o build/main.o build/video.o build/keyboard.o build/string.o build/fat.o build/interrupt.o build/floppy.o build/rtc.o build/scancodes.o kernel.ld
	$(TARGET)-ld -Tkernel.ld -m elf_i386 --nmagic -nostdlib -static -o "build/kernel.elf" -Map symbols/kernel.map
build/utils.o : utils.asm
	nasm -g -felf32 -F dwarf utils.asm -o "build/utils.o"
build/main.o : main.c ../include/kernel/keyboard.h ../include/kernel/video.h ../include/kernel/utils.h ../include/kernel/interrupt.h ../include/kernel/floppy.h ../include/kernel/rtc.h
	$(TARGET)-gcc -c main.c -o "build/main.o" -m32 -mtune=i386 -gdwarf $(INC) $(MY_CFLAGS)
build/video.o : video.c ../include/kernel/main.h ../include/kernel/video.h
	$(TARGET)-gcc -c video.c -o "build/video.o" -m32 -mtune=i386 -gdwarf $(INC) $(MY_CFLAGS)
build/keyboard.o : keyboard.c ../include/kernel/keyboard.h ../include/kernel/scancodes.h ../include/kernel/video.h ../include/kernel/utils.h ../include/kernel/interrupt.h
	$(TARGET)-gcc -c keyboard.c -o "build/keyboard.o" -m32 -mtune=i386 -gdwarf $(INC) $(MY_CFLAGS)
build/interrupt.o : interrupt.c ../include/kernel/interrupt.h ../include/kernel/keyboard.h ../include/kernel/utils.h ../include/kernel/video.h
	$(TARGET)-gcc -c interrupt.c -o "build/interrupt.o" -m32 -mtune=i386 -gdwarf $(INC) $(MY_CFLAGS)
build/floppy.o : floppy.c ../include/kernel/floppy.h ../include/kernel/utils.h ../include/kernel/video.h ../include/kernel/interrupt.h ../include/kernel/rtc.h
	$(TARGET)-gcc -c floppy.c -o "build/floppy.o" -m32 -mtune=i386 -gdwarf $(INC) $(MY_CFLAGS)
build/rtc.o : rtc.c ../include/kernel/rtc.h ../include/kernel/utils.h ../include/kernel/interrupt.h 
	$(TARGET)-gcc -c rtc.c -o "build/rtc.o" -m32 -mtune=i386 -gdwarf $(INC) $(MY_CFLAGS)
build/scancodes.o : scancodes.c ../include/kernel/scancodes.h 
	$(TARGET)-gcc -c scancodes.c -o "build/scancodes.o" -m32 -mtune=i386 -DKEYB_TYPE_CH -gdwarf $(INC) $(MY_CFLAGS)
build/fat.o: fat.c ../include/kernel/fat.h ../include/kernel/string.h
	$(TARGET)-gcc -c fat.c -o "build/fat.o" -m32 -mtune=i386 -gdwarf $(INC) $(MY_CFLAGS)
build/string.o: string.c ../include/kernel/string.h
	$(TARGET)-gcc -c string.c -o "build/string.o" -m32 -mtune=i386 -gdwarf $(INC) $(MY_CFLAGS)
configure: 
	mkdir -p build
	mkdir -p symbols
clean:
	rm -rf build/*
	rm -rf symbols/*

	
