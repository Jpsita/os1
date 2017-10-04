ENTRY (entryShell);
SEARCH_DIR("../libKernel/build");
SECTIONS
{

	. = 0x40000;
	.text : AT(0x40000){
		build/shell.o(.text);
		libkernel.o(.text);
		utils.o(.text);
	}
	.data : {
		build/shell.o(.data);
		libkernel.o(.data);
		utils.o(.data);
	}
	.rodata : {
		build/shell.o(.rodata);
		build/shell.o(.rodata.*);
		libkernel.o(.rodata);
		libkernel.o(.rodata.*);
		utils.o(.rodata);
	}
	.bss : {
		build/shell.o(.bss);
		libkernel.o(.bss);
		utils.o(.bss);
	}
	.rel.text : {
		build/shell.o(.rel.text);
		libkernel.o(.rel.text);
		utils.o(.rel.text);
	}
	. = 0x120000;
	.eh_frame : AT(0x120000) {
		build/shell.o(.eh_frame);
		libkernel.o(.eh_frame);
		utils.o(.eh_frame);
	}
}
