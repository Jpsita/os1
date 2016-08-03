ENTRY(_main);
SECTIONS
{
	. = 0x008000;
	.text : AT(0x8000){
		build/utils.o(*);
		build/main.o(*);
		build/video.o(*);
		build/interrupt.o(*);
		build/keyboard.o(*);
	}
	. = 0x7C00;
	.init_code  :  AT(0x7C00)
	{
		build/entry.o(.text);
		build/entry.o(.tmpdata);
	}
	
	.sig : AT(0x7DFE)
	{
		SHORT(0xaa55);
	}
	. = 0x7E00;
	.data : AT(0x7E00)
	{
		build/entry.o(.data);
	}

}
