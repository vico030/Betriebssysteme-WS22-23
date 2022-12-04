#include "src/demo/aic_demo.h"
#include "src/lib/printf.h"
#include "src/drv/mc.h"
#include "src/drv/dbgu.h"
#include "src/sys/init.h"
#include "src/drv/aic.h"
#include "src/drv/st.h"


void mask_interrupt_bits_I_F() {
  asm volatile(
      "mrs r1, CPSR \n\t"
      "and r1, r1, #0b11111111111111111111111100111111 \n\t"
      "msr CPSR, r1 \n\t"
      );
}

__attribute__((section(".init")))
void _start(void) {
  // IRQ / FIQ enabler
  mask_interrupt_bits_I_F();

  // System Timer
  set_period_interval(34464);
  enable_period_interval_interrupt();

  // Advanced Interrupt Controller
  init_sys_smr();
  enable_sys_interrupt(); // last

  // Debug Unit
  init_DBGU();

  printf("\r\n");
  printf("*******************************\r\n");
  printf("*** Booting MentOS (R) 2022 ***\r\n");
  printf("*******************************\r\n");
  printf("\r\n");
  printf("Enable memory remap...\r\n");
  enable_MC_remap();

  init_system();

  // Demo Programs
  print_timed_output();


  for (;;);
}

