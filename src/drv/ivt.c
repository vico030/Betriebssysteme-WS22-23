//
// Created by marvolo on 19.11.22.
//

__attribute__((naked, section(".ivt")))
void ivt() {
  asm("b interrupt_handler \n\t");
  asm("b interrupt_handler ");
  asm("b interrupt_handler ");
  asm("b interrupt_handler ");
  asm("b interrupt_handler ");
  asm("nop");
  asm("b interrupt_handler ");
  asm("b interrupt_handler ");
}