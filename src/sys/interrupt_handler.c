//
// Created by marvolo on 06.12.22.
//

#include "interrupt_handler.h"
#include "../drv/aic.h"
#include "../drv/st.h"
#include "../drv/dbgu.h"
#include "../lib/printf.h"
#include "../sys/thread.h"

void normal_interrupt_handler() {
  asm volatile(
    //";- Adjust and save LR of IRQ mode in current stack"
      "sub  r14, r14, #4\n\t"
      // Scratch regs r0-r2
      "stmfd sp!, {r0-r2}\n\t"

      "mrs r0, SPSR\n\t" //IRQ_SPSR
      "mov r1, r14\n\t" //IRQ_LR
      "mov r2, r13\n\t" //IRP_SP

      //reset SP_IRQ
      "add r13, #12\n\t" // Todo: correct offset?

      // switch to SYS Mode, I-bit enabled
      "msr CPSR, #0b10011111\n\t"

      // push IRQ_LR to stack
      "stmfd sp!, {r1}\n\t"
      // push SYS_LR to stack
      "stmfd sp!, {r14}\n\t"
      // push r3-r12 to stack
      "stmfd sp!, {r3-r12}\n\t" // Reverse Order

      // push r0-r3 from IRQ_SP to stack
      "ldmfd r2!, {r3-r5}\n\t"
      "stmfd sp!, {r3-r5}\n\t"

      // push IRQ_SPSR to stack
      "stmfd sp!, {r0}\n\t"
      );

  register int sp asm ("r13");
  int stack_pointer = sp;

  // triggered by PITS when counter reached 0
  if (read_timer_status_register_PITS()) {
//    printf("!");
    //print_stack(stack_pointer, 16);
    //printf("SP before: 0x%x\r\n", stack_pointer);
    timer_unblock();
    switch_thread(&stack_pointer);
    //printf("SP after: 0x%x\r\n", stack_pointer);
    asm volatile(
        "mov SP, %[next_sp] \n\t"
        :
        : [next_sp] "r" (stack_pointer)
    :
    );
  }

  // triggered by DBGU when readable char
  else if (is_readable()) {
    // write in buffer
    push_to_lq();
    wake_thread(0); // activate thread spawner
  }


  end_interrupt_request_handling();
  asm volatile(
      // Restore SPSR
      "ldmfd sp!, {r0}\n\t"
      "msr cpsr, r0\n\t"

      "ldmfd sp!, {r0-r12, lr, pc}\n\t"
      );
}