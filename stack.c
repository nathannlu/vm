#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

void initialize(struct Stack *stack) {
  stack->sp = NULL;
}

// @todo 
bool is_full(const struct Stack *stack) {
  return false;
}

bool is_empty(const struct Stack *stack) {
  return stack->sp == NULL;
}

void push(struct Stack *stack, struct vm_value value) {
  if(is_full(stack)) {
    printf("Stack overflow");
    exit(EXIT_FAILURE);
  } else {
    if(is_empty(stack)) {
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

struct vm_value pop(struct Stack *stack) {
  if(is_empty(stack)) {
    printf("Stack underflow");
    exit(EXIT_FAILURE);
  } else {
    struct vm_value popped_value = *stack->sp;

    // move stack pointer down
    stack->sp--;

    // check if stack is empty
    if(stack->sp < &stack->data[0]) {
      stack->sp = NULL;
    }

    return popped_value; 
  }
}

void print_stack(const struct Stack* stack) {
  printf("Stack Pointer:%p\n", (void*)stack->sp);
  printf("Stack Memory Layout:\n");
  for (const struct vm_value *ptr = stack->data; ptr < stack->data + STACK_LIMIT; ++ptr) {

    printf("%p: ", (const void*)ptr);
    if (ptr >= stack->data && ptr <= stack->sp) {
      switch (ptr->type) {
        case NUMBER:
          printf("NUMBER(%f)\n", ptr->number);
          break;
        // Add cases for other types if needed
        default:
          printf("Unknown Type\n");
          break;
      }
    } else {
      printf("Uninitialized\n");
    }
  }
}
