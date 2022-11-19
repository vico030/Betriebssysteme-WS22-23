ENTRY(_start)
SECTIONS
{
	. = 0x20000000;
	.ivt : { *(.ivt) }
	.init : { *(.init) }
	.text : { *(.text) }
}




