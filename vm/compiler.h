#ifndef COMPILER_H
#define COMPILER_H

#include "op_code.h"
#include "value.h"
#include "ast.h"
#include <stdint.h>
#include <stdio.h>

#define MAX_BYTECODE_LENGTH 254
#define MAX_CONSTANT_LENGTH 10


/**
 * Converts AST into bytecode
 */
struct compiler {
  uint8_t* bytecode;
  int count;          // bytecode length

  struct vm_value constants[MAX_CONSTANT_LENGTH];
  int constants_length;   // always points to next empty slot in arr to be filled
};

void initialize_compiler(struct compiler* c) {
  // malloc bytecode;
  c->bytecode = (uint8_t*)malloc(sizeof(uint8_t) * MAX_BYTECODE_LENGTH);
  c->count = 0;

  c->constants_length = 0;
}

int compiler_add_constant(struct compiler* c, const struct vm_value constant) {
  if(c->constants_length == MAX_CONSTANT_LENGTH) {
    // realloc
  }

  int index = c->constants_length;
  c->constants[index] = constant;

  printf("adding constant: %f to index %d\n", constant.number, index);

  c->constants_length++;

  return index;
}

void compiler_patch_jmp_address(struct compiler* c, int tmp_addr_index, uint16_t patched_index) {
  c->bytecode[tmp_addr_index] = (patched_index >> 8) & 0xff;
  c->bytecode[tmp_addr_index+1] = patched_index & 0xff;
}

// add op_code to bytecode
void compiler_emit(struct compiler* c, uint8_t op_code) {
  if(c->count < MAX_BYTECODE_LENGTH) {
    c->bytecode[c->count] = op_code;
    c->count++; // always points to next empty slot in arr to be filled
  } else {
    printf("Max bytecode length reached");
  }
}

// recursive function to recursively generate
// bytecode from ast
void compiler_gen(struct compiler* c, struct ast_node* ast) {
  
  switch(ast->type) {
    case Program:

      compiler_gen(c, ast->Program.body);
      break;

    case ExpressionStatement:
      // handle expression



      // recursive
      /*
      for(int i = 0; i < ast->children_count; i++) {

        // @todo - fix this
        // this only works if all child
        // nodes are type body
        //compiler_gen(c, ast->body[i]);
      }*/

      break;

    case BinaryExpression:

      // Evaluate children first
      compiler_gen(c, ast->BinaryExpression.left);
      compiler_gen(c, ast->BinaryExpression.right);

      switch(ast->BinaryExpression.op) {
        // Calculations
        case ADD:
          compiler_emit(c, OP_ADD);
          break;
        case SUB:
          compiler_emit(c, OP_SUB);
          break;
        case MUL:
          compiler_emit(c, OP_MUL);
          break;
        case DIV:
          compiler_emit(c, OP_DIV);
          break;

        /**
         * BOOLEAN MAP
         * - 0 is <
         * - 1 is >
         * - 2 is ==
         * - 3 is >=
         * - 4 is <= 
         * - 5 is !=
         */
        case LESS_THAN:
          compiler_emit(c, OP_COMPARE);
          compiler_emit(c, 0);
          break;
        case GREATER_THAN:
          compiler_emit(c, OP_COMPARE);
          compiler_emit(c, 1);
          break;
        case EQUAL:
          compiler_emit(c, OP_COMPARE);
          compiler_emit(c, 2);
          break;
        case GREATER_EQUAL_THAN:
          compiler_emit(c, OP_COMPARE);
          compiler_emit(c, 3);
          break;
        case LESS_EQUAL_THAN:
          compiler_emit(c, OP_COMPARE);
          compiler_emit(c, 4);
          break;
        case NOT_EQUAL:
          compiler_emit(c, OP_COMPARE);
          compiler_emit(c, 5);
          break;
        /*
        case default:
          printf("BinaryExpression error: unknown operation");
          break;
        */
      }

      break;

    case IfStatement:
      // handle binary expression
      compiler_gen(c, ast->IfStatement.test);

      // Because OP_JMP_IF_FALSE looks ahead
      // in the bytecode for two address bits that
      // represent the index in the bytecode,
      // we emit two 0 bits as placeholder.
      // 
      // The placeholder bits will be patched
      // later in the code
      compiler_emit(c, OP_JMP_IF_FALSE);
      int jmp_to_alternate_code_index = c->count;      // this points to the beginning of the patch
      compiler_emit(c, 0);
      compiler_emit(c, 0);

      // (1)
      // Generate the consequent bytecode
      compiler_gen(c, ast->IfStatement.consequent);

      // this int describes the index where
      // the consequent code ends
      int consequent_code_end_index = c->count;

      // in consequent, you need to emit values that
      // jump past alternate
      compiler_emit(c, OP_JMP);
      int skip_alternate_code_index = c->count;      // this points to the beginning of the patch
      compiler_emit(c, 0);
      compiler_emit(c, 0);

      // (2)
      // Generate the alternate bytecode
      if (c->count > UINT16_MAX) {
        printf("IfStatement error: index larger than uint16_t");
      }
      uint16_t alternate_code_start_index = (uint16_t)c->count;

      compiler_gen(c, ast->IfStatement.alternate);

      // this int describes the index where
      // the alternate code ends
      if (c->count > UINT16_MAX) {
        printf("IfStatement error: index larger than uint16_t");
      }
      uint16_t alternate_code_end_index = (uint16_t)c->count;

      // (3)
      // Patching
      // make if jump false, jump to start of alternate code
      compiler_patch_jmp_address(c, jmp_to_alternate_code_index, alternate_code_start_index);

      // make if consequent code's jump, skip
      // alternate code, and jump to where it
      // ends
      compiler_patch_jmp_address(c, skip_alternate_code_index, alternate_code_end_index);

      break;

    case NumericLiteral:
      compiler_emit(c, OP_CONST);

      // add to constant array
      // constants.push
      int index = compiler_add_constant(c,NUMBER(ast->NumericLiteral.number));

      // emit the index to reference that variable
      compiler_emit(c, index);

      break;

    case StringLiteral:
      break;
  }
}

// @todo free mem of compiler


#endif
