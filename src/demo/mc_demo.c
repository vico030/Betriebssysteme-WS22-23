//
// Created by marvolo on 19.11.22.
//

#include "mc_demo.h"
#include "../lib/printf.h"
#include "../lib/io.h"

// check how to trigger interrupt -> access undefined memory
// trigger abort interrupt by accessing undefined memory address
void trigger_abort_interrupt() {

  printf("Now triggering abort interrupt...\r\n");

  write_u32(0x90000000, 2);

  printf("...");
}