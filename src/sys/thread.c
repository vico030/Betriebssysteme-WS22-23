//
// Created by marvolo on 13.12.22.
//

#include "thread.h"
#include "../lib/io.h"
#include "../lib/printf.h"
#include "../drv/dbgu.h"
#include "../drv/aic.h"

#define STACK_SIZE 0x1000
#define THREAD_AMOUNT 16
#define BASE_ADDRESS (STACK_SIZE * THREAD_AMOUNT + 0x6000)


enum status {
    READY,
    RUNNING,
    TERMINATED
};

typedef struct {
    unsigned int id;
    enum status state;
    unsigned int stack_pointer; // SP per Thread
} tcb;

typedef struct {
    tcb tcb_array[THREAD_AMOUNT];
    int tcb_current_id;
    int tcb_count;
} tcb_container;


tcb_container container;

int get_empty_tcb_slot() {
  if (container.tcb_count == 0) {
    //printf("+ found slot 0\r\n");
    return 0;
  }
  for (int i = ((container.tcb_current_id + 1) & (THREAD_AMOUNT - 1)); // Todo: Problematic loop on ID = -1!
       i != container.tcb_current_id;
       i = (i + 1) & (THREAD_AMOUNT - 1)) {
    // is tcb_array null?
    if (container.tcb_array[i].state == TERMINATED) {
      //printf("+ found slot %d\r\n", i);
      return i;
    }
  }
  printf("+ ERROR: No empty Thread\r\n");
  return -1;
}

void init_tcb_container(tcb_container *cont) {
  cont->tcb_current_id = -1;
  cont->tcb_count = 0;
  for (int i = 0; i < THREAD_AMOUNT; i++) {
    cont->tcb_array[i].state = TERMINATED;
  }
}

void idle(){
  //printf("i-");
  while (1) {
      asm volatile ("nop");
  }
}

void hello(){
  printf("Hello!");
}

void init_tcb_management() {
  init_tcb_container(&container);
  //create_thread((unsigned int ) &idle);
  //switch_thread();
}

void create_thread(unsigned int function_ptr) {
  //printf("+ creating thread...\r\n");
  int slot = get_empty_tcb_slot();
  if (slot != -1) {
    tcb *new_thread = &container.tcb_array[slot];
    new_thread->id = slot;
    new_thread->state = READY;

    // setup stack for context switch
    new_thread->stack_pointer = BASE_ADDRESS - slot * STACK_SIZE;
    new_thread->stack_pointer -= 4;
    write_u32(new_thread->stack_pointer, function_ptr);
    new_thread->stack_pointer -= 4;
    write_u32(new_thread->stack_pointer, (unsigned int) &delete_thread);
    for (int i = 0; i < 13; i++){
      new_thread->stack_pointer -= 4;
      write_u32(new_thread->stack_pointer, 0x0);
    }
    new_thread->stack_pointer -= 4;
    write_u32(new_thread->stack_pointer, 0b10000);

    container.tcb_count++;
    //printf("+ thread created\r\n");
    //printf("+ new thread state: %d, id: %d, sp: 0x%x, fp: 0x%x \r\n", new_thread->state, new_thread->id, new_thread->stack_pointer, function_ptr);
  }
}

void delete_thread() {
  //printf("- deleting thread...\r\n");
  // Todo: set all tcb values to 0
  container.tcb_array[container.tcb_current_id].state = TERMINATED;
  container.tcb_count--;
  if (container.tcb_count == 0){
    container.tcb_current_id = -1;
  }
  asm volatile ("mov SP, #0x5000"); // ????
  //printf("- thread deleted\r\n");
  // Return to idle mode
  idle();
}

int schedule_next_tcb() {
  if (container.tcb_current_id == -1) {
    for (int i = 0; i < THREAD_AMOUNT; i++) {
      enum status thread_state = container.tcb_array[i].state;

      if (thread_state == READY || thread_state == RUNNING) {
        return i;
      }
    }
  }

  for (int i = (container.tcb_current_id + 1) & (THREAD_AMOUNT - 1);  // current ID is skipped!
       i != container.tcb_current_id;
       i = (i + 1) & (THREAD_AMOUNT - 1)) {

    enum status thread_state = container.tcb_array[i].state;

    if (thread_state == READY || thread_state == RUNNING) {
      return i;
    }
  }
  // coming from existing thread
  // no other thread found
  return container.tcb_current_id;
}

void print_stack(unsigned int sp, unsigned int items){
  unsigned int tmp_sp = sp;
  printf("\r\nStack Pointer currently at: 0x%x\r\n", tmp_sp);
  printf("Last %d items on stack:\r\n", items);
  for (unsigned int i = 0; i < items; i++){
    printf("  0x%x\r\n", read_u32(tmp_sp));
    tmp_sp += 4;
  }
}



void switch_thread(int* stack_pointer) {
//  printf("~ Context Switch\r\n");
//  printf("~ tcb-count: %d\r\n", container.tcb_count);
//  printf("~ current tcb-ID: %d\r\n", container.tcb_current_id);

  // check for next ready/running thread
  if (container.tcb_count == 0) {
    return;
  }

  int next_tcb_id = schedule_next_tcb();

//  printf("~ next tcb-ID: %d\r\n", next_tcb_id);
  if (next_tcb_id == container.tcb_current_id) {
    return;
  }

  //register int stack_pointer asm ("r13");
  //print_stack(stack_pointer, 22);

  // save context if id != -1
  if (container.tcb_current_id != -1) {
    tcb *current_thread = &container.tcb_array[container.tcb_current_id]; // Todo: ID = -1
    current_thread->stack_pointer = *stack_pointer;
//    asm volatile(
//        "STR SP, %[current_sp] \n\t"
//        : [current_sp] "=&o"(current_thread->stack_pointer)
//    :
//    :
//    );
  }

  // load context
  tcb *next_thread = &container.tcb_array[next_tcb_id];
  write_u32((unsigned int) stack_pointer, next_thread->stack_pointer);
//  asm volatile(
//      "LDR SP, %[next_sp] \n\t"
//      :
//  : [next_sp] "o" (next_thread->stack_pointer)
//  :
//  );

//  asm volatile(
//      // switch stack pointers to enter next thread
//      "STR SP, %[current_sp] \n\t"
//      "LDR SP, %[next_sp] \n\t"
//      : [current_sp] "=&o"(current_thread->stack_pointer)
//  : [next_sp] "o" (next_thread->stack_pointer)
//  :
//  );

  // change TCB state
  enum status thread_state = container.tcb_array[next_tcb_id].state;
  // printf("~ tcb-state: %d\r\n", thread_state);
  if (thread_state == READY) {
    container.tcb_array[next_tcb_id].state = RUNNING;
  }

  // set container->tcb_current_id
  container.tcb_current_id = next_tcb_id;

  //print_stack(container.tcb_array[container.tcb_current_id].stack_pointer, 16);

  // print new line
  printf("\r\n");

//  end_interrupt_request_handling();
//  asm volatile(
//    // Restore SPSR
//      "ldmfd sp!, {r0}\n\t"
//      "msr cpsr, r0\n\t"
//
//      "ldmfd sp!, {r0-r12, lr, pc}\n\t"
//      );
}
