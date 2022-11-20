#include "src/demo/dbgu_demo.h"
#include "src/demo/mc_demo.h"
#include "src/lib/printf.h"
#include "src/drv/mc.h"
#include "src/drv/dbgu.h"
#include "src/sys/init.h"


__attribute__((section(".init")))
void _start(void) {
  enable_DBGU_transmit();
  printf("Enable memory remap...\r\n");
  enable_MC_remap();

  init_system();

  write_in_console();
  trigger_abort_interrupt();
  write_in_console();

  for (;;);
}

