#include "eval.h"
#include "global.h"
#include "value.h"

uint8_t *ip;

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


void run(uint8_t* bytecode, struct vm_value* constants) {
  /*
  struct eval eval;
  eval.co = alloc_main();
  eval.ip = co->co;
  */

  struct code_object* co = alloc_from_bytecode(bytecode, constants);
  ip = co->bytecode;
  struct Stack stack;
  initialize(&stack);
  struct callstack cstack;
  callstack_initialize(&cstack);


  struct vm_value result;   // variable for results popped off the stack
  uint8_t next_byte;        // variable to hold next byte in the bytecode
  uint16_t address_index;   // variable to hold next two bytes for jmp and jmp_if_false
  
  // variables for temp holding items that are popped off from the stack
  struct vm_value x;
  struct vm_value y;
  struct vm_value z;

  for(;;) {
    switch (read_byte()) {
      case OP_HALT:
        printf("instruction halt\n");
        return;

      case OP_CONST:
        printf("instruction const\n");
        // move instruction pointer to next byte
        // and get next value
        next_byte = read_byte();

       // printf("NUMBER(%f)\n", ptr->number);
        printf("reading constant: %f\n", co->constants[0].number);
        printf("reading constant: %f\n", co->constants[1].number);
        printf("reading constant: %f\n", co->constants[2].number);
        printf("reading constant: %f\n", co->constants[3].number);
        printf("reading constant: %f\n", co->constants[4].number);

        // push it to the stack
        push(&stack, co->constants[next_byte]);
        break;

      case OP_ADD:
        printf("instruction add\n");

        y = pop(&stack);
        x = pop(&stack);

        if (y.type  == NUMBER && x.type == NUMBER) {
          // convert a and b to doubles
          double double_a = AS_NUMBER(x);
          double double_b = AS_NUMBER(y);

          // add 
          result = NUMBER(double_a + double_b); 

          // put value back on the stack
          push(&stack, result);
        } else {
          // @todo free up str_a & str_b
          // this creates 3 mem blocks in the heap
          char *str_a = vm_value_to_string(x);
          char *str_b = vm_value_to_string(y);
          char *combined_string = (char*)allocate_heap(strlen(str_a) + strlen(str_b) + 1);

          strcpy(combined_string, str_a);
          strcat(combined_string, str_b);

          result = object_to_vm_value(new_object_string_object(combined_string));

          push(&stack, result);
        }

        break;
      case OP_SUB:
        printf("instruction sub\n");

        y = pop(&stack);
        x = pop(&stack);

        if (y.type  == NUMBER && x.type == NUMBER) {
          // convert a and b to doubles
          double double_a = AS_NUMBER(x);
          double double_b = AS_NUMBER(y);

          // sub
          result = NUMBER(double_a - double_b); 

          // put value back on the stack
          push(&stack, result);
        } else {
          printf("OP_SUB not supported for non-numbers");
        }

        break;
      case OP_MUL:
        printf("instruction mul\n");

        y = pop(&stack);
        x = pop(&stack);

        if (y.type  == NUMBER && x.type == NUMBER) {
          // convert a and b to doubles
          double double_a = AS_NUMBER(x);
          double double_b = AS_NUMBER(y);

          // sub
          result = NUMBER(double_a * double_b); 

          // put value back on the stack
          push(&stack, result);
        } else {
          printf("OP_MUL not supported for non-numbers");
        }

        break;
      case OP_DIV:
        printf("instruction div\n");

        y = pop(&stack);
        x = pop(&stack);

        if (y.type  == NUMBER && x.type == NUMBER) {
          // convert a and b to doubles
          double double_a = AS_NUMBER(x);
          double double_b = AS_NUMBER(y);

          // sub
          result = NUMBER(double_a / double_b); 

          // put value back on the stack
          push(&stack, result);
        } else {
          printf("OP_DIV not supported for non-numbers");
        }

        break;
      case OP_COMPARE:
        printf("instruction compare\n");

        next_byte = read_byte();

        // pop next two values off the stack
        y = pop(&stack);
        x = pop(&stack);

        if (x.type == NUMBER && y.type == NUMBER) {
          double double_a = AS_NUMBER(x);
          double double_b = AS_NUMBER(y);
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
          switch(next_byte) {
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

        // next two bytes are the address bit
        // read_short reads and combines
        // the next two bytes
        address_index = read_short();

        if(is_false) {
          // set the instruction pointer to 
          // the address bytes on the bytecode
          ip = &co->bytecode[address_index];
          printf("Jumping to bytecode index: %u, address: %p\n", address_index, ip);
        }
        break;
      case OP_JMP:
        // next two bytes are the address bit
        // read_short reads and combines
        // the next two bytes
        address_index = read_short();

        // set the instruction pointer to 
        // the address bytes on the bytecode
        ip = &co->bytecode[address_index];
        printf("Jumping to bytecode index: %u, address: %p\n", address_index, ip);
        break;

      case OP_GET_GLOBAL:
        printf("Instruction get global\n");
        // globals are stored in the global array context
        next_byte = read_byte();

        // look in globals pool for constant
        struct global g = co->globals[next_byte];

        // push constant value onto the stack
        push(&stack, *g.value);

        break;

      case OP_SET_GLOBAL:
        printf("Instruction set global\n");

        // here the next byte in the 
        // bytecode represents the index
        // in the globals arr
        uint8_t next_byte = read_byte();

        // get value from top of stack
        x = peek(&stack, 0);

        set_global(co->globals, next_byte, &x);

        break;

      case OP_CALL:
        printf("Instruction call func\n");
        // next_byte represents the arg count
        next_byte = read_byte();

        // get the function that is on the top
        // of the stack
        x = peek(&stack, next_byte);

        // @temp disable
        bool is_native_function = false;
        if(is_native_function) {
          /**
           * Native function
           */
          void (*func_handler)() = (void (*)())x.object->data;
          func_handler();

          // after you are done with func,
          // 1. pop result
          // 2. pop all args.
          //    ( + 1 for the func on stack too)
          // 3. push result back to value stack
          /*
          result = pop(&stack);
          for(int j = 0; j < next_byte + 1; j++) {
            pop(&stack);
          }
          push(&stack, result);
          */
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
          co = (struct code_object*)x.object->code;

          // @todo - implement arguments
          // set bp to the first argument of the func
          // e.g. arg1 = index 0, arg2 = index 1;
          
          // set instruction pointer to the beginning of new bytecode
          printf("Jumping to bytecode from (%p) to: %p\n", (void*)ip, (void*)&co->bytecode[0]);
          ip = &co->bytecode[0];


          //printf("got bytecode at: %p\n", (void*)cf->co);
        }

        break;

      case OP_GET_LOCAL:
        printf("instruction get local\n");
        // local_index describes the position of the local variable
        // (which exists on the stack) relative to the bp;
        //
        // since local variables are defined at the beginning of the 
        // stackframe and bp points to the start of the frame, you can
        // later retrieve the local variable by the offset of the bp
        //
        // @important
        // this assumes local variables are initialized at the 
        // start of the frame once we enter the frame
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
        printf("Instruction set local\n");
        uint8_t _local_index = read_byte();

        if(stack.bp == NULL) {
          printf("No values found\n");
        } else {
          // check the most recent value on the stack
          // and set the local (defined at the start
          // of the stack) with value.
          //
          // @important
          // this assumes local variables are initialized at the 
          // start of the frame once we enter the frame
          struct vm_value to_set_local = peek(&stack, 0);
          stack.bp[_local_index] = to_set_local;
        }

        break;

      case OP_SCOPE_EXIT:
        printf("Instruction scope exit\n");
        // Here, next_byte describes the
        // local variables in current scope
        // to be popped
        next_byte = read_byte();

        // after you are done with func,
        // 1. pop result
        // 2. pop all args.
        //    ( + 1 for the func on stack too)
        // 3. push result back to value stack
        result = pop(&stack);
        for(int p = 0; p < next_byte + 1; p++) {
          pop(&stack);
        }

        // @todo
        // instead of setting bp = NULL, move
        // bp to the next scope up
        stack.bp = NULL;

        push(&stack, result);

        break;

      case OP_RETURN:
        printf("Instruction return\n");
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
