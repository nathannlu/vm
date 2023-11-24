/**
 * Value stack implementation
 */

#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include "objects/value.h"

//#define STACK_LIMIT 512
#define STACK_LIMIT 10


struct Stack {
  struct vm_value   data[STACK_LIMIT];

  // stack pointer - points to the latest value in the stack.
  // if there is none points to null
  struct vm_value   *sp; 

  // base pointer - points to the
  // starting address of the stack
  struct vm_value   *bp; 
};

void initialize(struct Stack *stack);
bool is_full(const struct Stack *stack);
bool is_empty(const struct Stack *stack);
void push(struct Stack *stack, struct vm_value value);
struct vm_value pop(struct Stack *stack);
struct vm_value peek(struct Stack *stack, size_t offset);
void print_stack(const struct Stack* stack);

#endif 
