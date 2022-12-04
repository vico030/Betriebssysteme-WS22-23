//
// Created by Vico Tünke on 30.11.22.
//

#ifndef BETRIEBSSYSTEME_WS22_23_AIC_H
#define BETRIEBSSYSTEME_WS22_23_AIC_H

void init_sys_smr(void);
void enable_sys_interrupt(void);
void set_sys_handler_address(unsigned int);
void end_interrupt_request_handling(void);
void clear_sys_interrupt(void);

#endif //BETRIEBSSYSTEME_WS22_23_AIC_H
