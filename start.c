#include "src/demo/dbgu_demo.h"
#include "src/demo/mc_demo.h"
#include "src/lib/printf.h"
#include "src/drv/mc.h"
#include "src/drv/dbgu.h"
#include "src/sys/init.h"
#include "src/drv/aic.h"
#include "src/drv/st.h"
#include "src/lib/exception_handler.h"


void mask_interrupt_bits_I_F() {
  asm volatile(
      "mrs r1, CPSR \n\t"
      "and r1, r1, #0b11111111111111111111111100111111 \n\t"
      "msr CPSR, r1 \n\t"
      );
}

__attribute__((section(".init")))
void _start(void) {


  // System Timer

  mask_interrupt_bits_I_F();


  set_period_interval(34464);
  enable_period_interval_interrupt();

  // Advanced Interrupt Controller
  init_sys_smr();
  //set_sys_handler_address(0x00000018); // ToDo: Memory Map!
  enable_sys_interrupt(); // last


  // Debug Unit
  enable_DBGU_interrupt();
  enable_DBGU_transmit();
  enable_DBGU_receive();

  printf("\r\n");
  printf("******************************\r\n");
  printf("*** Booting GyrOS (R) 2022 ***\r\n");
  printf("******************************\r\n");
  printf("\r\n");
  printf("Enable memory remap...\r\n");
  enable_MC_remap();

  init_system();

  // Demo Programs
  //mc_demo();
  //write_in_console();

  for (;;);
}



