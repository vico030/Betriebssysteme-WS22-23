//
// Created by Vico Tünke on 30.11.22.
//

#ifndef BETRIEBSSYSTEME_WS22_23_ST_H
#define BETRIEBSSYSTEME_WS22_23_ST_H

#define PITS_TIME_PERIOD 34464

void enable_period_interval_interrupt(void);

void set_period_interval(unsigned short);

unsigned int read_timer_status_register_PITS(void);
#endif //BETRIEBSSYSTEME_WS22_23_ST_H
