//
// Created by marvolo on 19.11.22.
//

#include "mc_demo.h"
#include "../lib/printf.h"
#include "../lib/io.h"
#include "../drv/dbgu.h"
#include "../sys/thread.h"
#include "../demo/aic_demo.h"

// check how to trigger interrupt -> access undefined memory
// trigger abort interrupt by accessing undefined memory address
void trigger_abort_interrupt() {

  printf("Now triggering abort interrupt...\r\n");

  write_u32(0x90000000, 2);

  printf("...\r\n");
}

void trigger_software_interrupt() {

  printf("Now triggering software interrupt...\r\n");

  asm("SWI #12");

  printf("...\r\n");
}

void trigger_undefined_instruction_interrupt() {

  printf("Now triggering undefined instruction in...\r\n");

  asm("UDF #0xde11"); // thumb illegal instruction 0xde11

  printf("...\r\n");
}


void mc_demo(){
  printf("Enter one of the following to trigger an interrupt, bro.\r\n");
  printf("   1 - Data Abort\r\n");
  printf("   2 - Software Interrupt\r\n");
  printf("   3 - Undefined Instruction\r\n");
  printf("   $ - Exit input mode \r\n");

  int loop = 1;

  while(loop) {
    char read_char = read_character();
    switch(read_char){
      case '$':
        loop = 0;
        printf("You exited input mode. :)\r\n");
        break;
      case '1':
        trigger_abort_interrupt();
        break;
      case '2':
        trigger_software_interrupt();
        break;
      case '3':
        trigger_undefined_instruction_interrupt();
        break;
      default:
        printf("Please insert a valid interrupt\r\n");
        break;
    }
  }
}
char swi_read_char(){
  asm("swi #2");
  register char character asm ("r1");
  return character;
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

void read_character_via_SWI(){
  char input =  peek_at_lq();
  asm ("mov r1, %[read_char] \n\t"
  :
  : [read_char] "r" (input)
  :
  );
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



