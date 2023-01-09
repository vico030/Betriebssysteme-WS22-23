//
// Created by Vico Tünke on 19.11.22.
//

#include "exception_handler.h"
#include "../lib/printf.h"
#include "../drv/mc.h"


void stop_execution() {
  printf("Execution has terminated.\r\n");
  asm volatile(
      "mrs r0, cpsr\n\t"
      "orr r0, r0, #(1 << 7)\n\t" // Disable IRQ
      "msr cpsr_c, r0\n\t"
      "mov pc, lr\n\t"
      );
  while(1) continue;
}

__attribute__((section(".generic_interrupt_handler")))
void generic_interrupt_handler() {
  register int link_register asm("r14");
  printf("Generic Interrupt received at 0x%x.\r\n", link_register);
  stop_execution();
}

__attribute__((section(".data_abort_handler")))
void data_abort_handler() {
  asm volatile(
      "sub  r14, r14, #4 \n\t"
      "stmfd SP!, {r0-r12, r14}  \n\t"
      );

  register int link_register asm("r14");
  printf("Data Abort received at 0x%x while accessing 0x%x.\r\n", link_register - 4, get_abort_address());

  //asm volatile("ldmfd SP!, {r0-r12, PC}^");

  stop_execution();
}

__attribute__((section(".software_interrupt_handler")))
void software_interrupt_handler() {
  asm volatile(
      //"sub  r14, r14, #4 \n\t"
      "stmfd SP!, {r0-r12, r14}  \n\t"
      );

  register int link_register asm ("r14");
  printf("Software Interrupt received at 0x%x.\r\n", link_register - 4);

  //asm volatile("ldmfd SP!, {r0-r12, PC}^");
  stop_execution();
}


__attribute__((section(".undefined_instruction_handler")))
void undefined_instruction_handler() {
  asm volatile(
      //"sub  r14, r14, #4 \n\t"
      "stmfd SP!, {r0-r12, r14}  \n\t"
      );

  register int link_register asm ("r14");
  printf("Undefined Instruction received at 0x%x.\r\n", link_register - 4);

  //asm volatile("ldmfd SP!, {r0-r12, PC}^");

  stop_execution();
}