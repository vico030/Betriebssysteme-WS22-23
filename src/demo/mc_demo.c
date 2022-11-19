//
// Created by marvolo on 19.11.22.
//

#include "mc_demo.h"
#include "../drv/dbgu.h"
#include "../drv/mc.h"
#include "../lib/printf.h"
// check how to trigger interrupt -> access undefined memory
// trigger abort interrupt by accessing undefined memory address
void trigger_abort_interrupt() {

  enable_DBGU_transmit();
//  enable_DBGU_receive();

  printf("Enable memory remap...\r\n");
  enable_MC_remap();

  trigger_interrupt();

  printf("Now triggering interrupt...\r\n");
  *(volatile unsigned int*) 0x90000000 = 2;

  printf("...");
}