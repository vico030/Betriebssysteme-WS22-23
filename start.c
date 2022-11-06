#include "src/demo/dbgu_demo.h"

__attribute__((naked, section(".init")))
void _start(void)
{
  write_in_console();
  for(;;);
}

