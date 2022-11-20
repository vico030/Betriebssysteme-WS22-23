#include "src/demo/dbgu_demo.h"
#include "src/demo/mc_demo.h"
#include "src/lib/printf.h"
#include "src/drv/mc.h"
#include "src/drv/dbgu.h"
#include "src/sys/init.h"


__attribute__((section(".init")))
void _start(void) {
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

  mc_demo();

  for (;;);
}

