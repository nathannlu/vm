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

// @todo
// does this actually delete the item from the stack?
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

struct vm_value peek(struct Stack *stack, size_t offset) {
  if(is_empty(stack)) {
    printf("Stack underflow");
    exit(EXIT_FAILURE);
  } else {
    return *(stack->sp  - offset);
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
        case BOOLEAN:
          if(ptr->boolean == true) {
            printf("BOOLEAN(true)\n");
          } else {
            printf("BOOLEAN(false)\n");
          }

          break;
        // Add cases for other types if needed
        case OBJECT:
          if(ptr->object->type == STRING) {
            printf("OBJECT(%s)\n", (char*)ptr->object->data);

          } 
          else if(ptr->object->type == FUNCTION) {
            printf("FUNCTION()\n");
          }
          else {
            printf("Unknown Type\n");
          }

          break;
         
        default:
          printf("Unknown Type\n");
          break;
      }
    } else {
      printf("Uninitialized\n");
    }
  }
}
