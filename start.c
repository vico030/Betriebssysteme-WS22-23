#include "src/demo/dbgu_demo.h"
#include "src/drv/remap.h"

__attribute__((section(".init")))
void _start(void) {
    enable_MC_remap();
    write_in_console();
    for (;;);
}

