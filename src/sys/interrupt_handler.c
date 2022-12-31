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
      "stmfd sp!, {r14}\n\t"

      //";- Save SPSR and r0 in IRQ stack"
      "mrs r14, SPSR\n\t"
      "stmfd  sp!, {r0, r14}\n\t"

      //";- Read Modify Write the CPSR to Enable the Core Interrupt"
      //";- and Switch in SYS Mode ( same LR and stack as USR Mode )"
      "mrs r14, CPSR\n\t"
      "bic r14, r14, #0b10000000\n\t" // #I_BIT
      "orr r14, r14, #0b11111\n\t" // #ARM_MODE_SYS
      "msr CPSR, r14\n\t"

      //";- Save used registers and LR_usr in the System/User Stack"
      "stmfd sp!, {r1-r12, r14}\n\t"
      );

//  asm volatile(
//    //";- Adjust and save LR of IRQ mode in current stack"
//      "sub  r14, r14, #4\n\t"
//      "stmfd sp!, {r0-r12, r14}\n\t"
//
//      "mrs r14, SPSR\n\t"
//      "stmfd sp!, {r14}\n\t"
//
//      // Now, get SP_SYS from SYS Mode
//      "mrs r0, CPSR \n\t"
//      "orr r0, r0, #0b11111 \n\t"
//      "msr CPSR, r0\n\t"
//      "stm r1!, {sp} \n\t"
//      // Return from SYS Mode to IRQ Mode
//      "bic r0, r0, #0b10010010 \n\t"
//      "msr CPSR, r0\n\t"
//      // r1 (SP_SYS), SP_IRQ (r0-r12, r14_IRQ, SPSR_IRQ)
//      "mov r14, sp  \n\t"
//      // Change SP_IRQ to SP_SYS
//      "mov sp, r1 \n\t"
//      // Restore Values
//      "ldmfd r14!, {r0}\n\t"
//      "msr SPSR, r0\n\t"
//
//      "ldmfd r14!, {r0-r12, r14}\n\t"
//      "stmfd sp!, {r0-r12}\n\t"
//      );

  //  void* next_pc = &idle;

  // triggered by PITS when counter reached 0
  if (read_timer_status_register_PITS()) {
    printf("!");
    //switch_thread();

    // save r0-r12 (incl.)
    // save r14 (LR) for return AFTER IRQ handling (contains entry point of new thread)
    // save SPSR (mode) from prev. execution
    // Total 15
    // IGNORE: PC, CPSR, SP (-> context)

    // swap SP old for SP new

    // load registers in reverse order

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

  asm volatile(
      // ";- Restore used registers and LR_usr from the System/User Stack"
      "ldmfd sp!, {r1-r12, r14}\n\t"

      //";- Read Modify Write the CPSR to disable interrupts"
      //";- and to go back in the mode corresponding to the exception"
      "mrs r0, CPSR\n\t"
      "bic r0, r0, #0b11111\n\t" // #ARM_MODE_SYS
      "orr r0, r0, #0b10010010\n\t" // #I_BIT:OR:ARM_MODE_IRQ
      "msr CPSR, r0\n\t"
      );

  // ;- Mark the End of Interrupt on the interrupt controller
  end_interrupt_request_handling();

  asm volatile(
      //";- Restore SPSR_irq and r0 from the IRQ stack"
      "ldmfd sp!, {r0, r14}\n\t"
      "msr SPSR, r14\n\t"

      //";- Restore ajusted LR_irq from IRQ stack directly in the PC"
      "ldmfd sp!, {pc}^\n\t" //Todo: Set PC to pointer in TCB

      // Trial and error...
      //"pop {r14}\n\t"
//      "ldmfd sp!, {lr}\n\t"
//      "ldr lr, #%[next]\n\t"
//      "bx lr\n\t"
//      ::[next] "r" (next_pc):"lr"
      );
}