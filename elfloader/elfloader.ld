ENTRY (entryElf);
SEARCH_DIR("../libKernel/build");
SECTIONS
{
	. = 0x37200;
	.text : AT(0x37200){
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
		libkernel.o(.text);
		libkernel.o(.data);
		libkernel.o(.bss);
		libkernel.o(.rodata);
		libkernel.o(.rel.text);
		utils.o(.text);
		utils.o(.data);
		utils.o(.bss);
		utils.o(.rodata);
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