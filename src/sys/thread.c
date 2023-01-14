//
// Created by marvolo on 13.12.22.
//

#include "thread.h"
#include "../lib/io.h"
#include "../lib/printf.h"
#include "../drv/st.h"

#define STACK_SIZE 0x1000
#define THREAD_AMOUNT 16
#define BASE_ADDRESS (STACK_SIZE * THREAD_AMOUNT + 0x6000)


enum status {
    READY,
    RUNNING,
    TERMINATED,
    SLEEPING,
    BLOCKED,
};

typedef struct {
    unsigned int id;
    enum status state;
    unsigned int stack_pointer; // SP per Thread
    int unblock_time;
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

void idle() {
  //printf("i-");
  while (1) {
    asm volatile ("nop");
  }
}

void init_tcb_management() {
  init_tcb_container(&container);
  //create_thread((unsigned int ) &idle);
  //switch_thread();
}

unsigned int initialize_stack(unsigned int stack_pointer, unsigned int start_function, unsigned int end_function,
                              unsigned int mode) {
  stack_pointer -= 4;
  write_u32(stack_pointer, start_function);
  stack_pointer -= 4;
  write_u32(stack_pointer, end_function);
  for (int i = 0; i < 13; i++) {
    stack_pointer -= 4;
    write_u32(stack_pointer, 0x0);
  }
  stack_pointer -= 4;
  write_u32(stack_pointer, mode);
  return stack_pointer;
}

void set_stack_entry(unsigned int stack_pointer, unsigned int offset, unsigned int value){
  write_u32(stack_pointer + offset, value);
}

void create_thread(unsigned int function_ptr) {
  //printf("+ creating thread...\r\n");
  int slot = get_empty_tcb_slot();
  if (slot != -1) {
    tcb *new_thread = &container.tcb_array[slot];
    new_thread->id = slot;
    new_thread->state = READY;
    new_thread->unblock_time = 0;

    new_thread->stack_pointer = initialize_stack(
        BASE_ADDRESS - slot * STACK_SIZE,
        function_ptr,
        (unsigned int) &delete_thread,
        0b10000
    );

    container.tcb_count++;
    //printf("+ thread created\r\n");
    //printf("+ new thread state: %d, id: %d, sp: 0x%x, fp: 0x%x \r\n", new_thread->state, new_thread->id, new_thread->stack_pointer, function_ptr);
  }
}

void create_thread_with_arg(unsigned int function_ptr, unsigned int argument) {
  int slot = get_empty_tcb_slot();
  if (slot != -1) {
    tcb *new_thread = &container.tcb_array[slot];
    new_thread->id = slot;
    new_thread->state = READY;
    new_thread->unblock_time = 0;

    new_thread->stack_pointer = initialize_stack(
        BASE_ADDRESS - slot * STACK_SIZE,
        function_ptr,
        (unsigned int) &delete_thread,
        0b10000
    );

    // set r0 to value so that it is read as function argument when thread starts
    set_stack_entry(new_thread->stack_pointer, 4, argument);
//    print_stack(new_thread->stack_pointer, 16);

    container.tcb_count++;
  }
}

void delete_thread() {
  //printf("- deleting thread...\r\n");
  // Todo: set all tcb values to 0
  container.tcb_array[container.tcb_current_id].state = TERMINATED;
  container.tcb_count--;
  if (container.tcb_count == 0) {
    container.tcb_current_id = -1;
  }
  asm volatile ("mov SP, #0x5000"); // ????
  //printf("- thread deleted\r\n");
  // Return to idle mode
  idle();
}

void sleep_thread(int id) {
  container.tcb_array[id].state = SLEEPING;
}

void wake_thread(int id) {
  container.tcb_array[id].state = RUNNING;
}


int schedule_next_tcb() {
  if (container.tcb_current_id == -1) {
    for (int i = 0; i < THREAD_AMOUNT; i++) {
      enum status thread_state = container.tcb_array[i].state;

      if (thread_state == READY || thread_state == RUNNING) {
        return i;
      }
    }
  } else {
    for (int i = (container.tcb_current_id + 1) & (THREAD_AMOUNT - 1);  // current ID is skipped!
         i != container.tcb_current_id;
         i = (i + 1) & (THREAD_AMOUNT - 1)) {

      enum status thread_state = container.tcb_array[i].state;

      if (thread_state == READY || thread_state == RUNNING) {
        return i;
      }
    }
  }
  // coming from existing thread
  // no other thread found
  return container.tcb_current_id;
}

void print_stack(unsigned int sp, unsigned int items) {
  unsigned int tmp_sp = sp;
  printf("\r\nStack Pointer currently at: 0x%x\r\n", tmp_sp);
  printf("Last %d items on stack:\r\n", items);
  for (unsigned int i = 0; i < items; i++) {
    printf("  0x%x\r\n", read_u32(tmp_sp));
    tmp_sp += 4;
  }
}

unsigned int create_dummy_stack() {
  return initialize_stack(0x5000,
                          (unsigned int) &idle,
                          (unsigned int) &idle,
                          0b10000);
}

void switch_thread(int *stack_pointer) {
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
    if (container.tcb_array[container.tcb_current_id].state == SLEEPING ||
        container.tcb_array[container.tcb_current_id].state == BLOCKED) {
      // create dummy stack to get into idle mode
      // save tcb and idle!!!
      tcb *current_thread = &container.tcb_array[container.tcb_current_id];
      current_thread->stack_pointer = *stack_pointer;
      write_u32((unsigned int) stack_pointer, create_dummy_stack());
      container.tcb_current_id = -1;
    }
    return;
  }

  // save context if id != -1
  if (container.tcb_current_id != -1) {
    tcb *current_thread = &container.tcb_array[container.tcb_current_id];
    current_thread->stack_pointer = *stack_pointer;
  }

  // load context
  tcb *next_thread = &container.tcb_array[next_tcb_id];
  write_u32((unsigned int) stack_pointer, next_thread->stack_pointer);

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
}

void timer_unblock() {
  if (container.tcb_count == 0) return;
  for (int i = 0; i < THREAD_AMOUNT; i++) {
    tcb tmp_tcb = container.tcb_array[i];
    if (tmp_tcb.state == BLOCKED) {
      tmp_tcb.unblock_time -= PITS_TIME_PERIOD;
      if (tmp_tcb.unblock_time <= 0) {
        tmp_tcb.state = RUNNING;
        tmp_tcb.unblock_time = 0;
      }
    }
  }
}

void timer_block(int block_time) {
  container.tcb_array[container.tcb_current_id].state = BLOCKED;
  container.tcb_array[container.tcb_current_id].unblock_time = block_time;
}