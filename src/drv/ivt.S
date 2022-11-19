.section .ivt, "ax"

.global abt_handler


vector_table:
    b abt_handler ; Reset
    b abt_handler ; Unddefined Instruction
    b abt_handler ; Software Interrupt
    b abt_handler ; Prefetch Abort
    b abt_handler ; Data Abort
    nop
    b abt_handler ; IRQ
    b abt_handler ; FIQ
