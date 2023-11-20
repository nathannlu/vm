#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include "value.h"

//#define STACK_LIMIT 512
#define STACK_LIMIT 10

struct Stack {
  struct vm_value   data[STACK_LIMIT];
  struct vm_value   *sp; // stack pointer
};

void initialize(struct Stack *stack);
bool is_full(const struct Stack *stack);
bool is_empty(const struct Stack *stack);
void push(struct Stack *stack, struct vm_value value);
struct vm_value pop(struct Stack *stack);
void print_stack(const struct Stack* stack);

#endif 
