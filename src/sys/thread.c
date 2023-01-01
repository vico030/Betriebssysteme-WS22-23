//
// Created by marvolo on 13.12.22.
//

#include "thread.h"
#include "../lib/io.h"
#include "../lib/printf.h"
#include "../drv/dbgu.h"

#define STACK_SIZE 0x1000
#define THREAD_AMOUNT 16
#define BASE_ADDRESS (STACK_SIZE * THREAD_AMOUNT + 0x6000)


enum status {
    READY,
    RUNNING,
    WAITING,
    TERMINATED
};

typedef struct {
    unsigned int id;
    enum status state;
    unsigned int stack_pointer; // SP per Thread
    unsigned int mode;
    unsigned int return_address;
} tcb;

typedef struct {
    tcb tcb_array[THREAD_AMOUNT];
    int tcb_current_id;
    int tcb_count;
} tcb_container;


tcb_container container;

int get_empty_tcb_slot() {
  if (container.tcb_count == 0) {
    printf("+ found slot 0\r\n");
    return 0;
  }
  for (int i = ((container.tcb_current_id + 1) & (THREAD_AMOUNT - 1)); // Todo: Problematic loop on ID = -1!
       i != container.tcb_current_id;
       i = (i + 1) & (THREAD_AMOUNT - 1)) {
    // is tcb_array null?
    if (container.tcb_array[i].state == TERMINATED) {
      printf("+ found slot %d\r\n", i);
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
  printf("i-");
  while (1) {
      asm volatile ("nop");
  }
}

void hello(){
  printf("Hello!");
}

void init_tcb_management() {
  init_tcb_container(&container);
  create_thread((unsigned int ) &idle);
  //switch_thread();
}

void create_thread(unsigned int function_ptr) {
  printf("+ creating thread...\r\n");
  int slot = get_empty_tcb_slot();
  if (slot != -1) {
    tcb *new_thread = &container.tcb_array[slot];
    new_thread->id = slot;
    new_thread->state = READY;
    new_thread->stack_pointer = BASE_ADDRESS - slot * STACK_SIZE;
    new_thread->mode = 0b10000; // USR Mode

    container.tcb_count++;

    // push function pointer to SP
    //new_thread->stack_pointer--; // -= 4;
    write_u32(new_thread->stack_pointer, function_ptr);

    // push r0 - r14 and SPSR to SP (in this case all 0) // Todo: fix SPSR
//    for (int i = 1; i < 15; i++){
//      write_u32(new_thread->stack_pointer - i, 0);
//    }

    // set SP so all values can be popped on switch
    new_thread->stack_pointer -= (15*4); // 4 per register!
    printf("+ thread created\r\n");
    printf("+ new thread state: %d, id: %d, sp: 0x%x, fp: 0x%x \r\n", new_thread->state, new_thread->id,
           new_thread->stack_pointer, function_ptr);
  }
}

void delete_thread() {
  printf("- deleting thread...\r\n");
  // Todo: set all tcb values to 0
  container.tcb_array[container.tcb_current_id].state = TERMINATED;
  container.tcb_count--;
  printf("- thread deleted\r\n");
  // Todo: Return to normal execution / idle mode
}

int get_next_tcb() {
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

void switch_thread() {

  // NOTE: Old implementation! Doesn't work with current Register Saving routine in IRQ handler!

  printf("~ Context Switch\r\n");
  printf("~ tcb-count: %d\r\n", container.tcb_count);
  printf("~ current tcb-ID: %d\r\n", container.tcb_current_id);
  // check for next ready/running thread
  if (container.tcb_count == 0) {
    return;
  }

  int next_tcb_id = get_next_tcb();
  printf("~ next tcb-ID: %d\r\n", next_tcb_id);
  if (next_tcb_id == container.tcb_current_id) {
    return;
  }

  enum status thread_state = container.tcb_array[next_tcb_id].state;
  printf("~ tcb-state: %d\r\n", thread_state);

  // save context
  tcb *current_thread = &container.tcb_array[container.tcb_current_id]; // Todo: get SP from Register

  // load context
  tcb *next_thread = &container.tcb_array[next_tcb_id];

  asm volatile(
      "STMFD SP!, {r14} \n\t" // link register for interrupt
      "STMFD SP!, {r0-r14}^ \n\t" // user registers
      "MRS r2, spsr \n\t" // saved CPU state into R2
      "STMFD SP!, {r2} \n\t" // and then to stack

      // switch to other thread
      "STR SP, %[current_sp] \n\t" // pcb->cpu_state = SP,   r0 = external c variable
      "LDR SP, %[next_sp] \n\t" // SP = next_pcb->cpu_state
      : [current_sp] "=&o"(current_thread->stack_pointer)
  : [next_sp] "o" (next_thread->stack_pointer)
  :
  );


  // set container->tcb_current_id
  container.tcb_current_id = next_tcb_id;

  // print new line
  printf("\r\n");

  if (thread_state == RUNNING) {
    printf("~ Thread is in running state\r\n");
    asm volatile(
      // restore context
        "LDMFD SP!, {r2} \n\t" // CPU state to R2
        "MSR spsr, r2 \n\t" // and then into saved state
        "LDMFD SP!, {r0-r14}^ \n\t" // user registers
        "LDMFD SP!, {pc} \n\t" // link register for return from interrupt
        );

  } else if (thread_state == READY) {
    printf("~ Thread is in ready state\r\n");
    container.tcb_array[next_tcb_id].state = RUNNING;
    printf("~ Still alive...\r\n");
    printf("~ Next char in queue: c%\r\n", peek_at_lq());

    //void *delete_thread_address = &delete_thread;
    asm volatile(
        //set LR to delete function
        //"ldr LR, %[exit] \n\t"
        //"pop {PC}\n\t"
        "pop {LR}\n\t" // Todo: bl instead?
        :
        : //[exit] "o" (delete_thread_address)
        :
        );
  }

  printf("~ Still alive 2...\r\n"); // Todo: CRASH
  //delete_thread();
  // continue
  // Else (no thread) -> idle
}
