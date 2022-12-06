//
// Created by marvolo on 06.12.22.
//

#include "interrupt_handler.h"
#include "../drv/aic.h"
#include "../drv/st.h"
#include "../drv/dbgu.h"
#include "../lib/printf.h"


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

  end_interrupt_request_handling();
  asm volatile("ldmfd SP!, {r0-r12, PC}^");
  //clear_sys_interrupt();
}