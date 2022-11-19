.section .ivt, "ax"

.global interrupt_vector_reset

vector_table:
ldr PC, interrupt_vector_reset


interrupt_vector_reset:
.word 0x0