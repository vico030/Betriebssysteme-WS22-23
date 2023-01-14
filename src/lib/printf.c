#include "stdarg.h"
#include "../drv/dbgu.h"
#include "print.h"
#include "swi.h"


static char *convert_hex(unsigned int num, int base){
  static char hex_representation[]= "0123456789ABCDEF";
  static char buffer[50];
  char *ptr;

  ptr = &buffer[49];
  *ptr = '\0';

  do
  {
    *--ptr = hex_representation[num % base];
    num /= base;
  } while(num != 0);

  return(ptr);
}

static char *convert_dec(int num){
  static char dec_representation[]= "0123456789";
  static char buffer[50];
  char *ptr;
  int is_negative = 0;

  ptr = &buffer[49];
  *ptr = '\0';

  if (num < 0) {
    is_negative = 1;
    num *= -1;
  }

  do
  {
    *--ptr = dec_representation[num % 10];
    num /= 10;
  } while(num != 0);

  if(is_negative && (ptr >= &buffer[0]) ){
    *--ptr = '-';
  }

  return(ptr);
}

__attribute__((format(printf, 1, 2)))
void printf(char *fmt, ...) {
  char *traverse;
  unsigned int i;
  char *s;

  va_list args;
  va_start(args, fmt);

  traverse = fmt;

  while(*traverse != '\0')
  {
    char next_char = *traverse;
    traverse++;

    if(next_char != '%')
    {
      write_character(next_char);
//      swi_write_char(next_char);
      continue;
    }
    next_char = *traverse;
    traverse++;

    switch(next_char)
    {
      case 'c' :
        i = va_arg(args, unsigned int );
        write_character(i);
//        swi_write_char(i);
        break;

      case 's':
        s = va_arg(args, char *);
        print(s);
        break;

      case 'd':
        i = va_arg(args, int);
        print(convert_dec(i));
        break;

      case 'x':
        i = va_arg(args, unsigned int);
        print(convert_hex(i, 16));
        break;

      case 'p':
        i = (unsigned int) va_arg(args, void*);
        print(convert_hex(i, 16));
        break;
    }
  }
  va_end(args);
}