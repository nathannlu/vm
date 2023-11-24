/**
 * Value stack implementation
 */

#ifndef CALLSTACK_H
#define CALLSTACK_H

#include <stdbool.h>
#include "objects/value.h"
#include "objects/codeobject.h"

//#define STACK_LIMIT 512
#define STACK_LIMIT 10

struct frame {
  /**
   * Return address - uint8_t pointer to prev bytecode
   */
  uint8_t* ra;

  // @todo
  // implement base pointer
  void* bp;

  // previous code obj
  struct code_object* co;
};


struct callstack {
  struct frame data[STACK_LIMIT];

  // stack pointer - points to the latest value in the stack.
  // if there is none points to null
  struct frame *sp; 
};

void          callstack_initialize(struct callstack *stack);
bool          callstack_is_full(const struct callstack *stack);
bool          callstack_is_empty(const struct callstack *stack);
void          callstack_push(struct callstack *stack, struct frame value);
struct frame  callstack_pop(struct callstack *stack);
struct frame  callstack_peek(struct callstack *stack, size_t offset);
void          callstack_print_stack(const struct callstack* stack);

#endif 
