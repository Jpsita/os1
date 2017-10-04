ENTRY (entryElf);
SEARCH_DIR("../libKernel/build");
SECTIONS
{
	. = 0x50000;
	.text : AT(0x50000){
		build/main.o(.text);
		build/elfFuncs.o(.text);
		libkernel.o(.text);
		utils.o(.text);
	}
	.data : AT ( ADDR (.text) + SIZEOF (.text) ) {
		build/main.o(.data);
		build/elfFuncs.o(.data);
		libkernel.o(.data);
		utils.o(.data);
	}
	.bss : AT ( ADDR (.data) + SIZEOF (.data) ) {
		build/main.o(.bss);
		build/elfFuncs.o(.bss);
		libkernel.o(.bss);
		utils.o(.bss);
	}
	.rodata : AT ( ADDR (.bss) + SIZEOF (.bss) ) {
		build/main.o(.rodata);
		build/elfFuncs.o(.rodata);
		libkernel.o(.rodata);
		utils.o(.rodata);
	}
	.rel.text : AT ( ADDR (.rodata) + SIZEOF (.rodata) ) {
		build/main.o(.rel.text);
		build/elfFuncs.o(.rel.text);
		libkernel.o(.rel.text);
		utils.o(.rel.text);
	}

	. = 0x120000;
	.eh_frame : AT(0x120000) {
		build/main.o(.eh_frame);
		build/elfFuncs.o(.eh_frame);
		libkernel.o(.eh_frame);
		utils.o(.eh_frame);
	}
}
