#include "src/demo/aic_demo.h"
#include "src/lib/printf.h"
#include "src/drv/mc.h"
#include "src/drv/dbgu.h"
#include "src/sys/init.h"
#include "src/drv/aic.h"
#include "src/drv/st.h"
#include "src/sys/interrupt_handler.h"
#include "src/sys/thread.h"
#include "src/demo/swi_demo.h"


void mask_interrupt_bits_I_F() {
  asm volatile(
      "mrs r1, CPSR \n\t"
      "and r1, r1, #0b11111111111111111111111100111111 \n\t"
      "msr CPSR, r1 \n\t"
      );
}

__attribute__((section(".init")))
void _start(void) {

  printf("\r\n");
  printf("*******************************\r\n");
  printf("*** Booting MentOS (R) 2022 ***\r\n");
  printf("*******************************\r\n");
  printf("\r\n");

  printf("Initializing SVC mode stack... ");
  asm volatile ("mov SP, #0x24000000");
  printf("Done.\r\n");
  init_stacks();

  printf("Setting up interrupts... ");
  mask_interrupt_bits_I_F();
  set_sys_handler_address((unsigned int) &normal_interrupt_handler);
  printf("Done.\r\n");

  printf("Setting up period interval timer... ");
  set_period_interval(PITS_TIME_PERIOD);
  enable_period_interval_interrupt();
  printf("Done.\r\n");

  printf("Setting up SYS interrupts... ");
  init_sys_smr();
  enable_sys_interrupt();
  printf("Done.\r\n");


  printf("Setting up Debug Unit... ");
  init_DBGU();
  printf("Done.\r\n");

  printf("Enable memory remap... ");
  enable_MC_remap();
  printf("Done.\r\n");

  printf("Setting up TCB container... ");
  init_tcb_management();
  printf("Done.\r\n");

  create_thread((unsigned int) &thread_spawner);

  printf("Entering User Mode... ");
  asm volatile("msr CPSR, #0b10000\n\t");
  printf("Done.\r\n");

//  printf("trigger swi \r\n");
//  asm("swi #4\n");

  idle();

  while(1);
}

