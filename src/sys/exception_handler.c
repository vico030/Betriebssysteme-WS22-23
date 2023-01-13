//
// Created by Vico Tünke on 19.11.22.
//

#include "exception_handler.h"
#include "../lib/printf.h"
#include "../lib/io.h"
#include "../drv/mc.h"
#include "thread.h"
#include "../demo/mc_demo.h"


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

// TODO: REMOVE
void start(){
  while (1) {
    printf("Hallo test thread function");
  }
}
__attribute__((section(".software_interrupt_handler")))
void software_interrupt_handler() {
  asm volatile(
      //"sub  r14, r14, #4 \n\t"
      "stmfd SP!, {r0-r12, r14} \n\t"
      );

  register int link_register asm ("r14");
  int swi_type = read_u32(link_register - 4) & 0xFF;

  switch (swi_type) {
    case 1:
      // write character
      printf("swi: %d\r\n",swi_type);
      break;
    case 2:
      // read character
//      read_
      printf("swi: %d\r\n",swi_type);
      read_character_via_SWI();
      break;
    case 3:
      // delete thread
      printf("swi: %d\r\n",swi_type);
//      delete_thread();
      break;
    case 4:
      // create thread
      printf("swi: %d\r\n",swi_type);
      //create_thread((unsigned int)&start);
      create_thread_via_SWI();
      break;
    case 5:
      // time lock thread
      printf("swi: %d\r\n",swi_type);
      break;
    default:
      // stop execution
      printf("Invalid Software Interrupt %d received at 0x%x.\r\n", swi_type, link_register - 4);
      stop_execution();
      break;
  }



  asm volatile(
      "ldmfd SP!, {r0-r12, PC}^\n\t"
      );
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