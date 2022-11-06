//
// Created by marvolo on 06.11.22.
//

#include "dbgu_test.h"
#include "dbgu.h"
#include "print.h"

void write_in_console(void){
  // Initialize DBGU
  enable_DBGU_transmit();
  enable_DBGU_receive();


  char character = 'C';
  char string[] = "Linnert";
  int hex = 233; // == E9
  void * vptr = &hex;

  char character_format[] = "Hello World! Character: %c was printed.\r\n";
  char string_format[] = "Hello World! String: %s was printed.\r\n";
  char hex_format[] = "Hello World! Hex: %x was printed.\r\n";
  char void_format[] = "Hello World! Void: %p was printed.\r\n";


  printf(character_format, character);
  printf(string_format, string);
  printf(hex_format, hex);
  printf(void_format, vptr);


  printf("Enter $ to quit, bro.\r\n");
  while(1) {
    char read_char = read_character();
    if(read_char == '$') {
      printf("You exited input mode. :)\r\n");
      break;
    }
    printf("You entered the following character: %c\r\n", read_char);
  }
}