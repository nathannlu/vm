#ifndef COMPILER_H
#define COMPILER_H

#include "op_code.h"
#include "value.h"
#include "global.h"
#include "local.h"
#include "codeobject.h"
#include "ast.h"
#include <stdint.h>
#include <stdio.h>

#define MAX_CONSTANT_LENGTH 10
#define MAX_BYTECODE_LENGTH 254


/**
 * Converts AST into bytecode
 */
struct compiler {
  int               count;          // bytecode length
  struct code_object* co;

  int               constants_length;   // always points to next empty slot in arr to be filled
  size_t            scope_level;


  // deprecate these
  struct vm_value*  constants;
  struct globals*   globals;
  struct locals*    locals;
  uint8_t*          bytecode;
};

void initialize_compiler(struct compiler* c, struct vm_value* constants, struct globals* globals, struct locals* locals);
//int compiler_add_constant(struct compiler* c, const struct vm_value constant);
//void compiler_patch_jmp_address(struct compiler* c, int tmp_addr_index, uint16_t patched_index);
void compiler_emit(struct compiler* c, uint8_t op_code);
void compiler_gen(struct compiler* c, struct ast_node* ast);

#endif
