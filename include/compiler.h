#ifndef COMPILER_H
#define COMPILER_H

#include "codeobject.h"
#include "ast.h"

#define MAX_CONSTANT_LENGTH 10
#define MAX_BYTECODE_LENGTH 254

/**
 * Converts AST into bytecode
 */
struct compiler {
  struct code_object* co;
  int                 count;          // bytecode length

  int                 constants_length;   // always points to next empty slot in arr to be filled
  size_t              scope_level;
};

void initialize_compiler(struct compiler* c);
void compiler_emit(struct compiler* c, uint8_t op_code);
void compiler_gen(struct compiler* c, struct ast_node* ast);

#endif
