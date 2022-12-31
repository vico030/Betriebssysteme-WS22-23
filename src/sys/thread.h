//
// Created by marvolo on 13.12.22.
//

#ifndef BETRIEBSYSTEME_WS22_23_THREAD_H
#define BETRIEBSYSTEME_WS22_23_THREAD_H

void create_thread(unsigned int);
void switch_thread(void);
void delete_thread(void);
//void init_tcb_container();
void init_tcb_management ();
void idle(void);
void hello(void);

#endif //BETRIEBSYSTEME_WS22_23_THREAD_H
