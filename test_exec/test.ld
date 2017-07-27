ENTRY (main);
SECTIONS
{
	. = 0x00 + SIZEOF_HEADERS;
	.text : { *(.text) }
	.data : { *(.data) }
	.rodata : { *(.rodata) }
	.bss : { *(.bss) }
	.rel.text : { *(.rel.) }
}