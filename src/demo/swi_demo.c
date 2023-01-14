//
// Created by marvolo on 14.01.23.
//

#include "swi_demo.h"
#include "../lib/swi.h"
#include "../drv/dbgu.h"
#include "../sys/thread.h"
#include "../lib/printf.h"

void thread_spawner(){
  printf("Characters in queue: %d\r\n", get_size_content());
  while(1) {
    for (int i = 0; i < get_size_content(); i++){
//      char character = swi_read_char();
      printf("Characters in queue: %d\r\n", get_size_content());
      swi_read_char(); // read here to decrement size and break loop
      swi_create_thread('F');
    }
    sleep_thread(0);
  }
}


void active_wait(char character) {
  if(character != '\0') {
    for (int i = 0; i < 20; i++) {
      printf("%c", character);
      for (int j = 0; j < 99999999; j++) {
        asm volatile("nop");
      }
    }
  }
}

void passive_wait(char character){
  for (int i = 0; i < 20; i++) {
    printf("%c", character);
    timer_block(9999999);
  }
}

void print_and_wait(char character){
  printf("Current character: %c \r\n", character);
  if (character >= 'a' && character <= 'z'){
    passive_wait(character);
  }
  else if(character >= 'A' && character <= 'Z'){
    active_wait(character);
  }
  else{
    printf("Input %c is not a letter!\r\n", character);
  }
}

