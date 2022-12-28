//
// Created by marvolo on 13.12.22.
//

#include "thread.h"
#include "../lib/io.h"
#include "../lib/printf.h"

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
} tcb;

typedef struct {
    tcb tcb_array[THREAD_AMOUNT];
    int tcb_current_id;
    int tcb_count;
} tcb_container;


tcb_container container;

// TCB linked List

int get_empty_tcb_slot() {
  if (container.tcb_count == 0) return 0;
  for (int i = ((container.tcb_current_id + 1) & (THREAD_AMOUNT - 1));
       i != container.tcb_current_id;
       i = (i + 1) & (THREAD_AMOUNT - 1)) {
    // is tcb_array null?
    if (container.tcb_array[i].state == TERMINATED) {
      return i;
    }
  }
  return -1;
}

void init_tcb_container(tcb_container *cont) {
  cont->tcb_current_id = 0;
  cont->tcb_count = 0;
  for (int i = 0; i < THREAD_AMOUNT; i++) {
    cont->tcb_array[i].state = TERMINATED;
  }
}

void init_tcb_management (){
  init_tcb_container(&container);
}

void create_thread(unsigned int function_ptr) {
  printf("creating thread...\r\n");
  int slot = get_empty_tcb_slot();
  printf("found slot %d\r\n", slot);
  if (slot != -1) {
    tcb* new_thread = &container.tcb_array[slot];
    new_thread->id = slot;
    new_thread->state = READY;
    new_thread->stack_pointer = BASE_ADDRESS - slot * STACK_SIZE;

    container.tcb_count++;

    // push function pointer to SP
    new_thread->stack_pointer -= 4;
    write_u32(new_thread->stack_pointer, function_ptr);

    // push r0 - r14 and SPSR to SP (in this case all 0) // Todo: potential boom - SPSR auf 0?
//    for (int i = 1; i < 16; i++){  // Todo: 17 correct offset? 16?
//      write_u32(new_thread->stack_pointer - i, 0);
//    }
//    // set SP so all values can be popped on switch
//    new_thread->stack_pointer -= 16;
    printf("thread created\r\n");
    printf("new thread state: %d, id: %d, sp: 0x%x, fp: 0x%x \r\n",new_thread->state, new_thread->id, new_thread->stack_pointer, function_ptr);
  }
}

void delete_thread() {
  printf("deleting thread...\r\n");
  // Todo: set all tcb values to 0
  container.tcb_array[container.tcb_current_id].state = TERMINATED;
  container.tcb_count--;
}

void switch_thread() {
  // check for next ready/running thread
  for (int i = (container.tcb_current_id + 1) & (THREAD_AMOUNT - 1);  // Todo: current ID is skipped!
    i != container.tcb_current_id;
    i = (i + 1) & (THREAD_AMOUNT - 1)) {

    enum status thread_state = container.tcb_array[i].state;

    if (thread_state == READY || thread_state == RUNNING) {
      printf("TCB no. %d.\r\n", i);

      // save context
      tcb* current_thread = &container.tcb_array[container.tcb_current_id]; // Todo: -1 not possible!!!!!!!

      // load context
      tcb* next_thread = &container.tcb_array[i];
      printf("Still alive 1...\r\n");

      asm volatile(
          "STMFD SP!, {r14} \n\t" // link register for interrupt
          "STMFD SP!, {r0-r14}^ \n\t" // user registers
          "MRS r2, spsr \n\t" // saved CPU state into R2
          "STMFD SP!, {r2} \n\t" // and then to stack

          // switch to other thread
//          "STR SP, [r0] \n\t" // pcb->cpu_state = SP,   r0 = external c variable
//          "LDR SP, [r1] \n\t" // SP = next_pcb->cpu_state

          "STR SP, %[current_sp] \n\t" // pcb->cpu_state = SP,   r0 = external c variable
          "LDR SP, %[next_sp] \n\t" // SP = next_pcb->cpu_state
          : [current_sp] "=&o" (current_thread->stack_pointer)
          : [next_sp] "o" (next_thread->stack_pointer)
          :);


      // set container->tcb_current_id
      container.tcb_current_id = i;

      if (thread_state == RUNNING) {
        printf("Thread is in running state");
        asm volatile(
          // restore context
            "LDMFD SP!, {r2} \n\t" // CPU state to R2
            "MSR spsr, r2 \n\t" // and then into saved state
            "LDMFD SP!, {r0-r14}^ \n\t" // user registers
            "LDMFD SP!, {pc} \n\t" // link register for return from interrupt
            );
      } else if (thread_state == READY) {
        printf("Thread is in ready state");

        asm volatile(
            "POP {PC}"
            );
      }

      printf("Still alive 2...\r\n"); // Todo: CRASH
      // set current_thread->tcb_state RUNNING
      // print new line
      printf("\r\n");
      // continue
      break;
    }
  }
  // Else (no thread) -> idle
}
