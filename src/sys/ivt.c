
__attribute__((naked, section(".ivt")))
void ivt() {
  asm("nop");                                      // Reset
  asm("b undefined_instruction_handler");
  asm("bl software_interrupt_handler");
  asm("b generic_interrupt_handler");              // Prefetch Abort
  asm("b data_abort_handler");
  asm("nop");
  asm("LDR PC,[PC,# -0xF20]");               // IRQ  -> LDR PC,[PC,# -&F20] for AIC handling (IVR handler)
  asm("LDR PC,[PC,# -0xF20]");              // FIQ  -> LDR PC,[PC,# -&F20] for AIC handling (IVR handler)
}