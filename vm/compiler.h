#ifndef COMPILER_H
#define COMPILER_H

#include "op_code.h"
#include "value.h"
#include "ast.h"

#define MAX_BYTECODE_LENGTH 254
#define MAX_CONSTANT_LENGTH 254


/**
 * Converts AST into bytecode
 */
struct compiler {
  uint8_t* bytecode;
  int count;          // bytecode length

  struct vm_value constants[MAX_CONSTANT_LENGTH];
  int constants_length;
};

void initialize_compiler(struct compiler* c) {
  // malloc bytecode;
  c->bytecode = (uint8_t*)malloc(sizeof(uint8_t) * MAX_BYTECODE_LENGTH);
  c->count = 0;

  c->constants_length = 0;
}

int compiler_add_constant(struct compiler* c, struct vm_value* constant) {
  if(c->constants_length == MAX_CONSTANT_LENGTH) {
    // realloc
  }

  int index = c->constants_length;
  c->constants[index] = *constant;

  c->constants_length++;

  return index;
}

// add op_code to bytecode
void compiler_emit(struct compiler* c, uint8_t op_code) {
  if(c->count < MAX_BYTECODE_LENGTH) {
    c->bytecode[c->count] = op_code;
    c->count++;
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

        /*
        case default:
          printf("BinaryExpression error: unknown operation");
          break;
        */
      }

      break;

    case NumericLiteral:
      compiler_emit(c, OP_CONST);

      // add to constant array
      // constants.push
      int index = compiler_add_constant(c,&NUMBER(ast->NumericLiteral.number));

      // emit the index to reference that variable
      compiler_emit(c, index);

      break;

    case StringLiteral:
      break;
  }


}

// @todo free mem of compiler


#endif
