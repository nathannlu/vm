#ifndef COMPILER_H
#define COMPILER_H

#include "op_code.h"
#include "value.h"
#include "global.h"
#include "ast.h"
#include <stdint.h>
#include <stdio.h>

#define MAX_BYTECODE_LENGTH 254
#define MAX_CONSTANT_LENGTH 10


/**
 * Converts AST into bytecode
 */
struct compiler {
  uint8_t*          bytecode;
  int               count;          // bytecode length
  struct vm_value*  constants;
  struct globals*   globals;

  int               constants_length;   // always points to next empty slot in arr to be filled

  //struct vm_value locals[MAX_CONSTANT_LENGTH];
};

void initialize_compiler(struct compiler* c, struct vm_value* constants, struct globals* globals);
int compiler_add_constant(struct compiler* c, const struct vm_value constant);
void compiler_patch_jmp_address(struct compiler* c, int tmp_addr_index, uint16_t patched_index);
void compiler_emit(struct compiler* c, uint8_t op_code);
void compiler_gen(struct compiler* c, struct ast_node* ast);

#endif
