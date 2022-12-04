//
// Created by Vico Tünke on 19.11.22.
//

#include "exception_handler.h"
#include "printf.h"
#include "../drv/mc.h"
#include "../drv/aic.h"
#include "../drv/st.h"
#include "../drv/dbgu.h"
#include "../lib/loop_queue.h"


void stop_execution() {
  printf("Execution has terminated.\r\n");
  while(1);
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


__attribute__((section(".normal_interrupt_handler")))
void normal_interrupt_handler() {
  asm volatile(
      "sub  r14, r14, #4 \n\t"
      "stmfd SP!, {r0-r12, r14}  \n\t");

  // triggered by PITS when counter reached 0
  if(read_timer_status_register_PITS()) {
    printf("!\r\n");
  }

  // triggered by DBGU when readable char
  if(is_readable()) {
    // write in buffer
    push_to_lq();
  }
  asm volatile("ldmfd SP!, {r0-r12, PC}^");
//  end_interrupt_request_handling();
//  clear_sys_interrupt();
}