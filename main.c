#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "op_code.h"
#include "stack.c"

uint8_t bytecode[] = {
  OP_CONST,
  0,                  // store the index of the value in the constant pool
  OP_CONST,
  1,
  OP_ADD,
  OP_HALT
};


uint8_t *ip; // instruction pointer - always points to the next instruction


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

  struct vm_value constants[10] = {
  //  NUMBER(42.0),
  //  NUMBER(64.0),
    ALLOC_STRING("hello"),
    ALLOC_STRING(", world"),
  };


  for(;;) {
    switch (read_byte()) {
      case OP_HALT:
        printf("instruction halt\n");
        return;

      case OP_CONST:
        printf("instruction const\n");

        // move instruction pointer to next byte
        // and get next value

        // @question
        // why do you need to push to stack if it already exists
        // in constant pool?
        uint8_t constant_pool_index = read_byte();
        struct vm_value x = constants[constant_pool_index];
        printf("constant pool value:%f\n", x);

        // push it to the stack
        push(&stack, x);
        break;

      case OP_ADD:
        printf("instruction add\n");


        struct vm_value b = pop(&stack);
        struct vm_value a = pop(&stack);

        if (a.type == NUMBER && b.type == NUMBER) {
          // convert a and b to doubles
          double double_a = AS_NUMBER(a);
          double double_b = AS_NUMBER(b);

          // add 
          double result = double_a + double_b; 

          // put value back on the stack
          push(&stack, NUMBER(result));
        } else {
          char *str_a = AS_STRING(a);
          char *str_b = AS_STRING(b);

          char *result = (char*)malloc(strlen(str_a) + strlen(str_b) + 1);

          strcpy(result, str_a);
          strcat(result, str_b);

          // @todo free up str_a & str_b
          // this creates 3 mem blocks in the heap

          // convert str to vm_value

          push(&stack, ALLOC_STRING(result));
        }

        break;
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
