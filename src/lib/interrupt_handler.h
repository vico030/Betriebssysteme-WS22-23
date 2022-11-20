//
// Created by Vico Tünke on 19.11.22.
//

#ifndef BETRIEBSSYSTEME_WS22_23_INTERRUPT_HANDLER_H
#define BETRIEBSSYSTEME_WS22_23_INTERRUPT_HANDLER_H

void generic_interrupt_handler();
void undefined_instruction_handler();
void software_interrupt_handler();
void data_abort_handler();

#endif //BETRIEBSSYSTEME_WS22_23_INTERRUPT_HANDLER_H
