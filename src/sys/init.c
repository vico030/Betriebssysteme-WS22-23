#include "../lib/printf.h"

#define I_BIT 1 << 7
#define F_BIT 1 << 6
#define ARM_MODE_FIQ    0b10001 | I_BIT | F_BIT
#define ARM_MODE_IRQ    0b10010 | I_BIT | F_BIT
#define ARM_MODE_SVC    0b10011 | I_BIT | F_BIT
#define ARM_MODE_ABT    0b10111 | I_BIT | F_BIT
#define ARM_MODE_UND    0b11011 | I_BIT | F_BIT
#define ARM_MODE_SYS    0b11111 | I_BIT | F_BIT

void init_stack_pointer(unsigned int mode, unsigned int ptr) {
  asm volatile(
      "mrs r0,   CPSR     \n\t" // Move from CPSR to r0 - save mode
      "msr CPSR, %[mode]  \n\t" // Move to CPSR from mode - enter mode
      "mov SP,   %[ptr]   \n\t" // Move ptr to SP - assign SP address for current mode
      "msr CPSR, r0       \n\t" // Move to CPSR from r0 - restore mode
      :
      : [mode] "r" (mode), [ptr] "r" (ptr) // Assign C variables to asm variables
  : "r0" // Clobber - reserve this register
  );

  printf("Initialized stack pointer for mode %d at 0x%x\r\n", mode, ptr);
}


void init_system(){
  printf("Initializing stacks...\r\n");
  init_stack_pointer(ARM_MODE_FIQ, 0x00001000);
  init_stack_pointer(ARM_MODE_IRQ, 0x00002000);
  // init_stack_pointer(ARM_MODE_SVC, 0x00003000); // crash
  init_stack_pointer(ARM_MODE_ABT, 0x00004000);
  init_stack_pointer(ARM_MODE_UND, 0x00005000);
  init_stack_pointer(ARM_MODE_SYS, 0x24000000);
  printf("Initialization done.\r\n");
}