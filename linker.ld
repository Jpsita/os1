ENTRY(_main);
SECTIONS
{

	. = 0x7C00;
	.init_code  :  AT(0x7C00) 	{
		build/entry.o(.text);
		build/entry.o(.tmpdata);
	}
	.sig : AT(0x7DFE) {
		SHORT(0xaa55);
	}
	. = 0x7E00;
	.data : AT(0x7E00) {
		build/entry.o(.data);
	}
	. = 0x008000;
	.text : AT(0x8000) {
		build/utils.o(.text);
		build/utils.o(.data);
		build/utils.o(.bss);
		build/utils.o(.rodata);
		build/utils.o(.rel.text);
		build/video.o(.text);
		build/video.o(.data);
		build/video.o(.bss);
		build/video.o(.rodata);
		build/video.o(.rel.text);
		build/interrupt.o(.text);
		build/interrupt.o(.data);
		build/interrupt.o(.bss);
		build/interrupt.o(.rodata);
		build/interrupt.o(.rel.text);
		build/keyboard.o(.text);
		build/keyboard.o(.data);
		build/keyboard.o(.bss);
		build/keyboard.o(.rodata);
		build/keyboard.o(.rel.text);
		build/scancodes.o(.text);
		build/scancodes.o(.data);
		build/scancodes.o(.rodata);
		build/scancodes.o(.bss);
		build/scancodes.o(.rel.text);
		build/main.o(.text);
		build/main.o(.data);
		build/main.o(.bss);
		build/main.o(.rodata);
		build/main.o(.rel.text);
		build/floppy.o(.text);
		build/floppy.o(.data);
		build/floppy.o(.bss);
		build/floppy.o(.rodata);
		build/floppy.o(.rel.text);
	}
	. = 0x011200;
	.eh_frame : AT(0x11200) {
		*.*(.eh_frame);
	}
	
}
