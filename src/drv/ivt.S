.section .ivt, "ax"

.data
var1: .word 3

.global _interrupt_vector_reset

_interrupt_vector_reset:
    ldr r0, adr_var1

adr_var1: .word var1