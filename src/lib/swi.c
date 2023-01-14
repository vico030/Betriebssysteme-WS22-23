//
// Created by marvolo on 14.01.23.
//

#include "swi.h"
#include "../sys/thread.h"
#include "../demo/aic_demo.h"
#include "../lib/printf.h"

char swi_read_char(){
  asm("swi #2");
  register char character asm ("r0");
  return character;
}

void swi_write_char(char character){
  register char output asm("r0");
  output = character;
  asm("swi #1");
}

void swi_create_thread(){
  printf("swi_create_thread \r\n");
  asm("swi #4");
  printf("swi_create_thread2 \r\n");
  register void* start asm("r2");
  printf("swi_create_thread3\r\n");
  printf("%d\r\n",start);
  create_thread((unsigned int)start);
}

void create_thread_via_SWI(){
  unsigned int start = (unsigned int)&print_swi_output;
  printf("START ADDRESS:%d\r\n",start);
  printf("create_thread_via_SWI_start \r\n");
  asm volatile ("mov r2, %[start_addr] \n\t"
      :
      : [start_addr] "r" (start)
  :
  );
  printf("create_thread_via_SWI_END \r\n");
  create_thread((unsigned int)start);

}

//void input_listener(){
//  while(1) {
//    for (int i = 0; i < dbgu_LQ.size_content; i++){
//      char character = swi_read_char();
//      swi_create_thread(&function(character));
//    }
//    sleep();
//  }
//}

