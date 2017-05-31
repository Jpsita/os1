ENTRY (entryElf);
SEARCH_DIR("../libKernel/build");
SECTIONS
{
	. = 0x11C00;
	.text : AT(0x11C00){
		build/main.o(.text);
		build/main.o(.data);
		build/main.o(.bss);
		build/main.o(.rodata);
		build/main.o(.rel.text);
		build/elfFuncs.o(.text);
		build/elfFuncs.o(.data);
		build/elfFuncs.o(.bss);
		build/elfFuncs.o(.rodata);
		build/elfFuncs.o(.rel.text);
		libkernel.a(.text);
		libkernel.a(.data);
		libkernel.a(.bss);
		libkernel.a(.rodata);
		libkernel.a(.rel.text);
	}

	. = 0x120000;
	.eh_frame : AT(0x120000) {
		*.*(.eh_frame);
	}
}