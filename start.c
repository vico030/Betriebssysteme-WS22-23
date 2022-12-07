#include "src/demo/aic_demo.h"
#include "src/lib/printf.h"
#include "src/drv/mc.h"
#include "src/drv/dbgu.h"
#include "src/sys/init.h"
#include "src/drv/aic.h"
#include "src/drv/st.h"
#include "src/sys/interrupt_handler.h"


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
  set_period_interval(34464);
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


  printf("\r\nStarting Demo Program... \r\n\r\n");
  print_timed_output();


  for (;;);
}

