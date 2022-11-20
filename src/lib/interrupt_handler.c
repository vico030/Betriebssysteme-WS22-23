//
// Created by Vico Tünke on 19.11.22.
//

#include "interrupt_handler.h"
#include "printf.h"

__attribute__((section(".generic_interrupt_handler")))
void generic_interrupt_handler() {
  register int link_register asm ("r14");
  printf("Generic Interrupt received at 0x%x.\r\n", link_register);
  while (1);
}

__attribute__((section(".data_abort_handler")))
void data_abort_handler() {
  asm(
      "sub  r14, r14, #4 \n\t"
      "stmfd SP!, {r0-r12, r14}  \n\t"
      );

  register int link_register asm ("r14");
  printf("Data Abort Interrupt received at 0x%x.\r\n", link_register - 4);

  asm("ldmfd SP!, {r0-r12, PC}^");
}

__attribute__((section(".software_interrupt_handler")))
void software_interrupt_handler() {
  asm(
      //"sub  r14, r14, #4 \n\t"
      "stmfd SP!, {r0-r12, r14}  \n\t"
      );

  register int link_register asm ("r14");
  printf("Software Interrupt received at 0x%x.\r\n", link_register - 4);

  asm("ldmfd SP!, {r0-r12, PC}^");

}

__attribute__((section(".undefined_instruction_handler")))
void undefined_instruction_handler() {
  asm(
      //"sub  r14, r14, #4 \n\t"
      "stmfd SP!, {r0-r12, r14}  \n\t"
      );

  register int link_register asm ("r14");
  printf("Undefined Instruction Interrupt received at 0x%x.\r\n", link_register - 4);

  asm("ldmfd SP!, {r0-r12, PC}^");
}