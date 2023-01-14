//
// Created by marvolo on 13.12.22.
//

#ifndef BETRIEBSYSTEME_WS22_23_THREAD_H
#define BETRIEBSYSTEME_WS22_23_THREAD_H

void create_thread(unsigned int);
void create_thread_with_arg(unsigned int,unsigned int);
void switch_thread(int*);
void delete_thread(void);
//void init_tcb_container();
void init_tcb_management ();
void idle(void);
void hello(void);
void print_stack(unsigned int, unsigned int);
void sleep_thread(int);
void wake_thread(int);
void timer_unblock(void);
void timer_block(int);
int get_current_thread_status(void);

#endif //BETRIEBSYSTEME_WS22_23_THREAD_H
