ENTRY (entryShell);
SEARCH_DIR("../libKernel/build");
SECTIONS
{

	. = 0x33200;
	.text : AT(0x33200){
		build/shell.o(.text);
		build/shell.o(.rel.text);
		libkernel.o(.text);
		utils.o(.text);
		libkernel.o(.rel.text);
		utils.o(.rel.text);
	}
	.data : {
		build/shell.o(.data);
		libkernel.o(.data);
		utils.o(.data);
	}
	.rodata : {
		build/shell.o(.rodata);
		libkernel.o(.rodata);
		utils.o(.rodata);
	}
	.bss : {
		build/shell.o(.bss);
		libkernel.o(.bss);
		utils.o(.bss);
	}
	. = 0x120000;
	.eh_frame : AT(0x120000) {
		build/shell.o(.eh_frame);
		libkernel.o(.eh_frame);
		utils.o(.eh_frame);
	}
}
