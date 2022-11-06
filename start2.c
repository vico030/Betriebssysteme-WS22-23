#include "src/dbgu_test.h"

__attribute__((naked, section(".init")))
void _start(void)
{
  write_in_console();
  for(;;);
}

