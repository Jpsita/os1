ENTRY(_main);
SECTIONS
{

	. = 0x7C00;
	.text  :  AT(0x7C00){
		build/bootloader.o(.text);
		build/bootloader.o(.data);
	}
	.sig : AT(0x7DFE) {
		SHORT(0xaa55);
	}
	. = 0x7E00;
	.eh_frame : AT(0x7E00){
		*.*(.eh_frame);
	}
}