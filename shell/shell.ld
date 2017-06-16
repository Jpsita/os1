ENTRY (entryShell);
SEARCH_DIR("../libKernel/build");
SECTIONS
{

	. = 0x14000;
	.text : AT(0x14000){
		build/shell.o(.text);
		build/shell.o(.data);
		build/shell.o(.bss);
		build/shell.o(.rodata);
		build/shell.o(.rel.text);
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
