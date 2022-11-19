//
// Created by Vico Tünke on 19.11.22.
//

#include "interrupt_handler.h"
#include "printf.h"

__attribute__((section(".handlers")))
void interrupt_handler() {
    printf("Interrupt received.\r\n");
}