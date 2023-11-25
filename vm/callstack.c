#include "callstack.h"
#include <stdio.h>
#include <stdlib.h>

void callstack_initialize(struct callstack *stack) {
  stack->sp = NULL;
}

// @todo 
bool callstack_is_full(const struct callstack *stack) {
  return false;
}

bool callstack_is_empty(const struct callstack *stack) {
  return stack->sp == NULL;
}

void callstack_push(struct callstack *stack, struct frame value) {
  if(callstack_is_full(stack)) {
    printf("Stack overflow");
    exit(EXIT_FAILURE);
  } else {
    if(callstack_is_empty(stack)) {
      // set the sp to the first item
      stack->sp = &stack->data[0];
    } else {
      // move stack pointer up
      stack->sp++;
    }

    // dereference and set value
    *stack->sp = value;
  }
}

// @todo
// does this actually delete the item from the stack?
struct frame callstack_pop(struct callstack *stack) {
  if(callstack_is_empty(stack)) {
    printf("Stack underflow");
    exit(EXIT_FAILURE);
  } else {
    struct frame popped_value = *stack->sp;

    // move stack pointer down
    stack->sp--;

    // check if stack is empty
    if(stack->sp < &stack->data[0]) {
      stack->sp = NULL;
    }

    return popped_value; 
  }
}

struct frame callstack_peek(struct callstack *stack, size_t offset) {
  if(callstack_is_empty(stack)) {
    printf("Stack underflow");
    exit(EXIT_FAILURE);
  } else {
    return *(stack->sp  - offset);
  }
}


void callstack_print_stack(const struct callstack* stack) {
  printf("Stack Pointer:%p\n", (void*)stack->sp);
  printf("Stack Memory Layout:\n");
  for (const struct frame *ptr = stack->data; ptr < stack->data + STACK_LIMIT; ++ptr) {

    if (ptr >= stack->data && ptr <= stack->sp) {
      printf("Call_stack co: %p\n", (const void*)ptr->co);
      printf("Call_stack bp: %p\n", (const void*)ptr->bp);
      printf("Call_stack bytecode address: %p\n", (const void*)ptr->ra);
    } else {
      printf("Uninitialized\n");
    }
  }
}
