#include "loop_queue.h"
#include "printf.h"

typedef loop_queue lq;

void lq_push(lq* self, char c){
  printf(">> pushed character %c\r\n", c);
  if(self->size_buffer > self->size_content){
    self->buffer[self->write_pointer] = c;
    self->write_pointer = (self->write_pointer + 1) & buffer_size;
    self->size_content++;
  }
}

char lq_pop(lq* self){
  if(self->size_content > 0){
    char c = self->buffer[self->read_pointer];
    self->read_pointer = (self->read_pointer + 1) & buffer_size;
    self->size_content--;
    return c;
  }
  return -1;
}


void test_lq(void){
  char test_buffer[256];
  loop_queue subject = {buffer_size, 0,0,0,test_buffer};

  if (subject.size_content != 0) printf("1. Assertion error size\r\n");
  lq_push(&subject,'a');
  if (subject.size_content != 1) printf("2. Assertion error size\r\n");
  if (!(lq_pop(&subject) & 'a')) printf("3. Assertion error char 'a'\r\n");

  printf("Tests done.\r\n");
}