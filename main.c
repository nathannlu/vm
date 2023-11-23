#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "bytecode/op_code.h"
#include "stack.c"

#include "global.h"



/**
 * testing bytecode: (set x (+ x 10))
 * result should be 20
 */
uint8_t bytecode[] = {
  OP_GET_GLOBAL,  3,
  OP_CONST,       0,                  // push NUMBER(42.0) onto the stack
  OP_CONST,       1,                  // push NUMBER(64.0) onto the stack
  OP_CALL,        2,                  // function call parameter
  OP_HALT
};
/*
uint8_t bytecode[] = {
  OP_CONST,
  0,                  // store the index of the value in the constant pool
  OP_CONST,
  1,
  OP_COMPARE,
  1,
  OP_JMP_IF_FALSE,
  0,
  0,
  OP_HALT
};
*/

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

uint16_t read_short() {
  // Increment the pointer by 2 to point to the next 16-bit value
  ip += 2;

  // Combine the two bytes into a 16-bit value
  //
  // Notes:
  // (ip[-2] << 8) shifts the byte to the left.
  // e.g. if ip[-2] is 0xAB, << 16...  0xAB -> 0xAB0000
  //
  // | ip[-1] combines the two bytes.
  // e.g. into 0xABCD00
  uint16_t result = (uint16_t)((ip[-2] << 8) | ip[-1]);


  // Return the result
  return result;
}

uint8_t* load_bytecode(const char* path) {
  // Open the file for reading
  FILE *file = fopen(path, "rb");

  if (file == NULL) {
    perror("Error opening file");
  }

  // Determine the size of the file
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  rewind(file);

  // Allocate memory for the array
  uint8_t *bytecode = (uint8_t *)malloc(file_size);

  if (bytecode == NULL) {
    perror("Memory allocation error");
    fclose(file);
  }

  // Read the file content into the array
  fread(bytecode, 1, file_size, file);

  // Close the file
  fclose(file);

  return bytecode;
}


void eval() {
  struct Stack stack;
  initialize(&stack);

  struct vm_value constants[10] = {
    NUMBER(42.0),
    NUMBER(64.0),
    //ALLOC_STRING("hello"),
    //ALLOC_STRING(", world"),
  };


  void test_func() {
    printf("hello world");
  }
  // @todo
  void sum_func() {
    struct vm_value b = pop(&stack);
    struct vm_value a = pop(&stack);

    // convert a and b to doubles
    double double_a = AS_NUMBER(a);
    double double_b = AS_NUMBER(b);

    // add 
    double result = double_a + double_b; 

    // put value back on the stack
    push(&stack, NUMBER(result));
  }

  struct global globals[10] = {
    {"x", NUMBER(10.0)},
    {"y", NUMBER(20.0)},
    {"square", ALLOC_FUNC((void*)&test_func)},
    {"sum", ALLOC_FUNC((void*)&sum_func)},
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
      case OP_COMPARE:
        printf("instruction compare\n");

        uint8_t comparison_operator = read_byte();

        struct vm_value v2 = pop(&stack);
        struct vm_value v1 = pop(&stack);

        if (v1.type == NUMBER && v2.type == NUMBER) {
          double double_a = AS_NUMBER(v1);
          double double_b = AS_NUMBER(v2);
          bool res;

          /**
           * BOOLEAN MAP
           * - 0 is <
           * - 1 is >
           * - 2 is ==
           * - 3 is >=
           * - 4 is <= 
           * - 5 is !=
           */
          switch(comparison_operator) {
            case 0:
              res = double_a < double_b;
              break;
            case 1:
              res = double_a > double_b;
              break;
            case 2:
              res = double_a == double_b;
              break;
            case 3:
              res = double_a >= double_b;
              break;
            case 4:
              res = double_a <= double_b;
              break;
            case 5:
              res = double_a != double_b;
              break;
          }

          push(&stack, BOOLEAN(res));
        } else {
          printf("error: comparing strings");
        }
        break;

      case OP_JMP_IF_FALSE:
        // check if value on stack has a false boolean
        bool is_false = AS_BOOLEAN(pop(&stack)) == false;

        if(is_false) {
          // next two bytes are the address bit
          // read_short reads and combines
          // the next two bytes
          uint16_t address_index = read_short();

          // set the instruction pointer to 
          // the address bytes on the bytecode
          ip = &bytecode[address_index];
          printf("Jumping to bytecode index: 0x%04X, address: %p\n", address_index, ip);
        }
        break;

      case OP_GET_GLOBAL:
        printf("instruction get global\n");
        // globals are stored in the global array context
        uint8_t global_index = read_byte();

        // look in globals pool for constant
        struct global result = globals[global_index];

        // push constant value onto the stack
        push(&stack, result.value);

        break;

      case OP_SET_GLOBAL:
        printf("instruction set global\n");
        uint8_t gi = read_byte();
        struct vm_value value = peek(&stack, 0);

        set_global(globals, gi, value);

        break;

      case OP_CALL:
        // get func call param number
        uint8_t args_count = read_byte();
        struct vm_value f = peek(&stack, args_count);

        // @todo fix - this only pops the amount
        // for now
        // --
        // you need to make this so it moves the function
        // to the top of the stack
        // --
        // then the function can pop
        // the values
        /*
        for (int i = 0; i < arity; i++) {
          pop(&stack);
        }
        */

        // execute func

        //struct vm_value f = pop(&stack);
        void (*func_handler)() = (void (*)())f.object->data;
        func_handler();


        // get params
        //struct vm_value value = peek(&stack, arity);

        // invoke function with parameters

        break;

      default:
        printf("Unknown opcode");

    }

    print_stack(&stack);
  }
}

void exec() {
  //bytecode = load_bytecode("bytecode/data.bin");
  ip = &bytecode[0];

  return eval();
}


int main() {
  printf("Bytes allocated: %zu\n",bytes_allocated);
  exec();
  printf("Bytes allocated: %zu\n",bytes_allocated);

  return 0;
}
