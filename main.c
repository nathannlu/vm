#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "op_code.h"
#include "stack.c"

uint8_t bytecode[] = {
  OP_CONST,
  0,                  // store the index of the value in the constant pool
  OP_CONST,
  1,
  OP_HALT
};


struct vm_value constants[10] = {
  NUMBER(42.0),
  NUMBER(64.0),
};


uint8_t *ip; // instruction pointer


/**
 * Returns byte and moves pointer to next value
 */
uint8_t read_byte() {
  // get value of instruction pointer
  uint8_t val = *ip;
  
  // move to next byte
  ip++;

  return val;
}

void eval() {
  struct Stack stack;
  initialize(&stack);

  for(;;) {
    switch (read_byte()) {
      case OP_HALT:
        printf("instruction halt");
        return;

      case OP_CONST:
        printf("instruction const");

        // move instruction pointer to next byte
        // and get next value
        ip++;

        // @question
        // why do you need to push to stack if it already exists
        // in constant pool?
        uint8_t constant_pool_index = *ip;
        struct vm_value x = constants[constant_pool_index];

        // push it to the stack
        push(&stack, x);
    }

    print_stack(&stack);
  }
}

void exec() {
  // set instruction pointer to first item in bytecode array
  ip = &bytecode[0];

  /*
  printf("first bytecode memory address\n");
  printf("Address: %p, Value: 0x%02X\n", (void*)ip, *ip);
  */
  // parse code into ast


  // compile ast into bytecode

  return eval();
}



int main() {
  exec();

  return 0;
}
