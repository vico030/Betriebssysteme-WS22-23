//
// Created by Vico Tünke on 19.11.22.
//

#include "interrupt_handler.h"
#include "printf.h"

__attribute__((section(".generic_interrupt_handler")))
void generic_interrupt_handler() {
  printf("Generic Interrupt received.\r\n");
  while (1);
}

// Todo: Data Abort
__attribute__((section(".data_abort_handler")))
void data_abort_handler() {
  asm(
      "SUB  R14, R14, #4 \n\t"
      "STMFD SP!, {R14}  \n\t"
      );

  register int link_register asm ("r14");
  printf("Data Abort Interrupt received at 0x%x.\r\n", link_register - 4);

  asm("LDMFD SP!, {PC}^");
}

// Todo: Software Interrupt
__attribute__((section(".software_interrupt_handler")))
void software_interrupt_handler() {
  printf("Software Interrupt received.\r\n");
  while (1);
}

// Todo: Undefined Instruction
__attribute__((section(".undefined_instruction_handler")))
void undefined_instruction_handler() {
  printf("Undefined Instruction Interrupt received.\r\n");
  while (1);
}