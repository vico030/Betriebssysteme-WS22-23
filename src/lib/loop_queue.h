//
// Created by marvolo on 04.12.22.
//

#ifndef BETRIEBSYSTEME_WS22_23_LOOP_QUEUE_H
#define BETRIEBSYSTEME_WS22_23_LOOP_QUEUE_H
#define buffer_size 256

typedef struct {
    unsigned int size_buffer;
    unsigned int size_content;
    unsigned int read_pointer;
    unsigned int write_pointer;
    char *buffer;
} loop_queue;

void lq_push(loop_queue* self, char c);
char lq_pop(loop_queue* self);
void test_lq();

#endif //BETRIEBSYSTEME_WS22_23_LOOP_QUEUE_H
