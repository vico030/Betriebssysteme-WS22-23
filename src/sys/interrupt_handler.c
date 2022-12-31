//
// Created by marvolo on 06.12.22.
//

#include "interrupt_handler.h"
#include "../drv/aic.h"
#include "../drv/st.h"
#include "../drv/dbgu.h"
#include "../lib/printf.h"
#include "../sys/thread.h"
#include "../demo/aic_demo.h"


void normal_interrupt_handler() {
  asm volatile(
    //";- Adjust and save LR of IRQ mode in current stack"
      "sub  r14, r14, #4\n\t"
      // Scratch regs r0-r3
      "stmfd sp!, {r0-r3}\n\t"

      "mrs r0, SPSR\n\t" //IRQ_SPSR
      "mov r1, r14\n\t" //IRQ_LR
      "mov r2, r13\n\t" //IRP_SP

      //reset SP_IRQ
      "add r13, #12\n\t"

      // switch to SYS Mode, I-bit disabled
      "msr CPSR, #0b10010010\n\t"

      // push IRQ_LR to stack
      "stmfd sp!, {r1}\n\t"
      // push SYS_LR to stack
      "stmfd sp!, {r14}\n\t"
      // push r3-r12 to stack
      "stmfd sp!, {r3-r12}\n\t"

      // push r0-r3 from IRQ_SP to stack
      "ldmfd r2!, {r3-r5}\n\t"
      "ldmfd sp!, {r3-r5}\n\t"

      // push IRQ_SPSR to stack
      "stmfd sp!, {r0}\n\t"
      );

  // triggered by PITS when counter reached 0
  if (read_timer_status_register_PITS()) {
    printf("!");
    //switch_thread();
  }

  // triggered by DBGU when readable char
  if (is_readable()) {
    //printf("DBGU IRQ\r\n");
    // write in buffer
    push_to_lq();
    //printf("Next char in queue: %c\r\n", peek_at_lq());

    // Todo: new Thread on character input
    //create_thread((unsigned int) &print_threaded_output);

    //next_pc = &hello;
  }

  end_interrupt_request_handling();
  asm volatile(
      // Restore SPSR
      "ldmfd sp!, {r0}\n\t"
      "msr spsr, r0\n\t"

      "ldmfd sp!, {r0-r12, lr, pc}^\n\t"
      );
}