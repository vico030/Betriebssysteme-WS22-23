
__attribute__((naked, section(".ivt")))
void ivt() {
  asm("nop");                                        // Reset
  asm("b undefined_instruction_handler");
  asm("b software_interrupt_handler");
  asm("b generic_interrupt_handler");               // Prefetch Abort
  asm("b data_abort_handler");
  asm("nop");
  asm("b normal_interrupt_handler");              // IRQ
  asm("b generic_interrupt_handler");              // FIQ
}