#include "src/demo/dbgu_demo.h"
#include "src/drv/remap.h"
__attribute__((naked, section(".init")))
void _start(void)
{
  trigger_abort_interrupt();
  write_in_console();
  for(;;);
}

