.section .ivt

.global vector_table
.global vector_table_end

vector_table:
	b interrupt_handler // Reset
	b interrupt_handler // Unddefined Instruction
	b interrupt_handler // Software Interrupt
	b interrupt_handler // Prefetch Abort
	b interrupt_handler // Data Abort
	nop
	b interrupt_handler // IRQ
	b interrupt_handler // FIQ
vector_table_end:
