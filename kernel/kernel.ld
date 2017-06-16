ENTRY(entryc);
SECTIONS
{
	. = 0x8e00;
	.text : AT(0x8e00) {
		build/main.o(.text);
		build/utils.o(.text);
		build/video.o(.text);
		build/interrupt.o(.text);
		build/keyboard.o(.text);
		build/scancodes.o(.text);
		build/rtc.o(.text);
		build/floppy.o(.text);
		build/fat.o(.text);
		build/string.o(.text);


		build/main.o(.rodata);
		build/main.o(.rel.text);


		build/utils.o(.rodata);
		build/utils.o(.rel.text);


		build/video.o(.rodata);
		build/video.o(.rel.text);


		build/interrupt.o(.rodata);
		build/interrupt.o(.rel.text);


		build/keyboard.o(.rodata);
		build/keyboard.o(.rel.text);


		build/scancodes.o(.rodata);
		build/scancodes.o(.rel.text);


		build/rtc.o(.rodata);
		build/rtc.o(.rel.text);


		build/floppy.o(.rodata);
		build/floppy.o(.rel.text);


		build/fat.o(.rodata);
		build/fat.o(.rel.text);


		build/string.o(.rodata);
		build/string.o(.rel.text);
	}
	.data : {
		build/main.o(.data);
		build/utils.o(.data);
		build/video.o(.data);
		build/interrupt.o(.data);
		build/keyboard.o(.data);
		build/scancodes.o(.data);
		build/rtc.o(.data);
		build/floppy.o(.data);
		build/fat.o(.data);
		build/string.o(.data);
	}
	.bss : {
		build/main.o(.bss);
		build/utils.o(.bss);
		build/video.o(.bss);
		build/interrupt.o(.bss);
		build/keyboard.o(.bss);
		build/scancodes.o(.bss);
		build/rtc.o(.bss);
		build/floppy.o(.bss);
		build/fat.o(.bss);
		build/string.o(.bss);
	}
	. = 0x801200;
	.eh_frame : AT(0x801200) {
		*.*(.eh_frame);
	}

}
