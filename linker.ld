ENTRY(_main);
SECTIONS
{
	. = 0x7C00;
	.text  :  AT(0x7C00)
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
	. = 0x008000;
	.c_code : AT(0x8000){
		build/main.o(*);
	}
	. = 0x008600;
	.utils : AT(0x8600){
		build/utils.o(*);
	}
}
