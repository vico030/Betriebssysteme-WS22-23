ENTRY(_start)
SECTIONS
{
	. = 0x00200000;
	.ivt : { *(.ivt) }

    . = 0x002E0000;
    .handlers : {
    *(.generic_interrupt_handler);
   	*(.data_abort_handler);
    *(.software_interrupt_handler);
    *(.undefined_instruction_handler);
    *(.normal_interrupt_handler);
    }


	. = 0x20000000;
	.init : { *(.init) }
	.text : { *(.text) }
}




