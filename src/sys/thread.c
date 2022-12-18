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


tcb_container container; // Todo: potential boom

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
    tcb* new_thread = &container.tcb_array[slot]; // Todo: potential boom
    new_thread->id = slot;
    new_thread->state = READY;
    new_thread->stack_pointer = BASE_ADDRESS - slot * STACK_SIZE;

    container.tcb_count++;

    // push function pointer to SP
    write_u32(new_thread->stack_pointer, function_ptr);
    // push r0 - r14 and SPSR to SP (in this case all 0) // Todo: potential boom - SPSR auf 0?
    for (int i = 1; i < 17; i++){
      write_u32(new_thread->stack_pointer - i, 0);
    }
    // set SP so all values can be popped on switch
    new_thread->stack_pointer -= 17;
    printf("thread created\r\n");
    printf("new thread state: %d, id: %d \r\n",new_thread->state, new_thread->id);
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
  for (int i = (container.tcb_current_id + 1) & (THREAD_AMOUNT - 1);  // Todo: ID 0 is skipped!
    i != container.tcb_current_id; // == instead != ???
    i = (i + 1) & (THREAD_AMOUNT - 1)) {

    if (container.tcb_array[i].state == READY || container.tcb_array[i].state == RUNNING) {
      printf(" TCB no. %d.\r\n", i);

      // save context
      tcb* current_thread = &container.tcb_array[container.tcb_current_id];

      // load context
      tcb* next_thread = &container.tcb_array[i];
      printf("Still alive...\r\n");

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

          // restore context
          "LDMFD SP!, {r2} \n\t" // CPU state to R2
          "MSR spsr, r2 \n\t" // and then into saved state
          "LDMFD SP!, {r0-r14}^ \n\t" // user registers
          "LDMFD SP!, {pc} \n\t" // link register for return from interrupt
          : [current_sp] "=&o" (current_thread->stack_pointer)
          : [next_sp] "o" (next_thread->stack_pointer)
          :"sp", "lr");

      printf("Still alive...\r\n");
      // set container->tcb_current_id
      container.tcb_current_id = i;
      // set current_thread->tcb_state RUNNING
      // print new line
      printf("\r\n");
      // continue
      break;
    }
  }
  // Else (no thread) -> idle
}
