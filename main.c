#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "heap/allocation.c"
#include "bytecode/op_code.h"
#include "objects/codeobject.h"
#include "stack.c"
#include "callstack.c"



uint8_t *ip; // instruction pointer - always points to the next instruction

/**
 * Returns byte and moves pointer to next value
 */
uint8_t read_byte() {
  // get value of instruction pointer
  uint8_t val = *ip;

  printf("read val: %u\n", val);
  
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
  struct code_object* co = alloc_main();
  ip = co->co;

  struct Stack stack;
  initialize(&stack);

  struct callstack cstack;
  callstack_initialize(&cstack);

  for(;;) {
    switch (read_byte()) {
      case OP_HALT:
        printf("instruction halt\n");
        return;

      case OP_CONST:
        printf("instruction const\n");

        // move instruction pointer to next byte
        // and get next value
        uint8_t constant_pool_index = read_byte();

        //uint8_t constant_pool_index = read_byte();
        struct vm_value x = co->constants[constant_pool_index];

        // push it to the stack
        push(&stack,x);
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
          ip = &co->co[address_index];
          printf("Jumping to bytecode index: 0x%04X, address: %p\n", address_index, ip);
        }
        break;

      case OP_GET_GLOBAL:
        printf("instruction get global\n");
        // globals are stored in the global array context
        uint8_t global_index = read_byte();

        // look in globals pool for constant
        struct global result = co->globals[global_index];

        // push constant value onto the stack
        push(&stack, result.value);

        break;

      case OP_SET_GLOBAL:
        printf("instruction set global\n");
        uint8_t gi = read_byte();
        struct vm_value value = peek(&stack, 0);

        set_global(co->globals, gi, value);

        break;

      case OP_CALL:
        printf("instruction call func\n");
        // get func call param number
        uint8_t args_count = read_byte();
        struct vm_value fn = peek(&stack, args_count);

        // @temp disable
        bool is_native_function = false;
        if(is_native_function) {
          /**
           * Native function
           */
          void (*func_handler)() = (void (*)())fn.object->data;
          func_handler();

          // after you are done with func,
          // 1. pop result
          // 2. pop all args.
          //    ( + 1 for the func on stack too)
          // 3. push result back to value stack
          struct vm_value r = pop(&stack);
          for(int j = 0; j < args_count + 1; j++) {
            pop(&stack);
          }
          push(&stack, r);
        } else {
          /**
           * User-defined functions
           */

          // change void* data to a code_object*
          struct code_object* prev_co = co;

          // @todo
          // save current context to call stack
          struct frame f;
          f.ra = ip;
          f.bp = stack.bp;
          f.co = co;

          callstack_push(&cstack, f);



          // load locals, globals, constants
          co = (struct code_object*)fn.object->data;

          // @todo - implement arguments
          // set bp to the first argument of the func
          // e.g. arg1 = index 0, arg2 = index 1;
          
          // set instruction pointer to the beginning of new bytecode
          printf("Jumping to bytecode from (%p) to: %p\n", (void*)ip, (void*)&co->co[0]);
          ip = &co->co[0];


          //printf("got bytecode at: %p\n", (void*)cf->co);
        }

        break;

      case OP_GET_LOCAL:
        printf("instruction get local\n");
        // globals are stored in the global array context
        // local_index describes the position of the local variable
        // (which exists on the stack) relative to the bp;
        uint8_t local_index = read_byte();

        // look in globals pool for constant
        if(stack.bp == NULL) {
          printf("No values found\n");
        } else {
          struct vm_value l_result = stack.bp[local_index];

          // push constant value onto the stack
          push(&stack, l_result);
        }

        break;

      case OP_SET_LOCAL:
        printf("instruction set local\n");

        // this is the value to set to stack
        //struct vm_value l_value = peek(&stack, 0);

        // li is the index for the value in constant arr?
        read_byte();
        //uint8_t locals_arr_index = read_byte();

        // @todo
        // check if value at bp is same scope, if it is not
        // then update bp

        break;

      case OP_SCOPE_EXIT:
        printf("instruction scope exit\n");
        uint8_t scope_vars_count = read_byte();

        // after you are done with func,
        // 1. pop result
        // 2. pop all args.
        //    ( + 1 for the func on stack too)
        // 3. push result back to value stack
        struct vm_value scoped_res = pop(&stack);
        for(int p = 0; p < scope_vars_count + 1; p++) {
          pop(&stack);
        }

        // @todo
        // instead of setting bp = NULL, move
        // bp to the next scope up
        stack.bp = NULL;

        push(&stack, scoped_res);

        break;

      case OP_RETURN:
        printf("instruction return\n");
        // get frame from top of stack
        struct frame caller_frame = callstack_pop(&cstack);


        printf("Jumping to bytecode from (%p) to: %p\n", (void*)ip, (void*)caller_frame.ra);
        co = caller_frame.co;
        ip = caller_frame.ra;
        stack.bp = caller_frame.bp;


      default:
        printf("Unknown opcode\n");

    }

    printf("---\n");
    printf("Instruction pointer: %p\n", (void*)ip);
    print_stack(&stack);
    printf("----------------------\n");
  }
}

void exec() {
  //bytecode = load_bytecode("bytecode/data.bin");

  return eval();
}


int main() {
  printf("Bytes allocated: %zu\n",bytes_allocated);
  exec();
  printf("Bytes allocated: %zu\n",bytes_allocated);

  return 0;
}
