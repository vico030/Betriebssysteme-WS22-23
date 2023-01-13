//
// Created by marvolo on 19.11.22.
//

#ifndef BETRIEBSYSTEME_WS22_23_MC_DEMO_H
#define BETRIEBSYSTEME_WS22_23_MC_DEMO_H

void trigger_abort_interrupt();
void trigger_software_interrupt();
void trigger_undefined_instruction_interrupt();
void mc_demo();
char swi_read_char();
void read_character_via_SWI();
void swi_create_thread();
void create_thread_via_SWI();
#endif //BETRIEBSYSTEME_WS22_23_MC_DEMO_H
