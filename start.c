#include "src/demo/dbgu_demo.h"
#include "src/demo/mc_demo.h"

__attribute__((section(".init")))
void _start(void) {
  write_in_console();
  trigger_abort_interrupt();
  for (;;);
}

