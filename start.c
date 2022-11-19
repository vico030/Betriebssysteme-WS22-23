#include "src/demo/dbgu_demo.h"
#include "src/demo/mc_demo.h"
#include "src/lib/printf.h"
#include "src/lib/io.h"

#define I_BIT 1 << 7
#define F_BIT 1 << 6
#define ARM_MODE_ABT 0b10111 //| I_BIT | F_BIT

void init_stack_pointer(unsigned int mode, unsigned int ptr) {
  asm volatile(
      "mrs r3,   CPSR     \n\t" // Move from CPSR to r3 - save mode
      "msr CPSR, %[mode]  \n\t" // Move to CPSR from mode - enter mode
      "mov SP,   %[ptr]   \n\t" // Move ptr to SP - assign SP address for current mode
      //      "mov %[ptr], #1      \n\t"
      "msr CPSR, r3       \n\t" //Move to CPSR from r3 - restore mode
      :
      : [mode] "r" (mode), [ptr] "r" (ptr) // Assign C variables to asm variables
  : "r3" // Clobber - reserve this register
  );

  printf("Initialized stack pointer for mode %d at 0x%x\r\n", mode, ptr);
}

__attribute__((section(".init")))
void _start(void) {
  init_stack_pointer(ARM_MODE_ABT, 0x00204000);

  write_in_console();
  trigger_abort_interrupt();
  write_in_console();

  for (;;);
}

