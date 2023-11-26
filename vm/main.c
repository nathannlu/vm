#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "value.h"
#include "codeobject.h"
#include "allocation.h"
#include "stack.h"
#include "callstack.h"
#include "op_code.h"

#include "eval.h"

#include "tokenizer.h"
#include "compiler.h"
#include "ast.h"


struct _allocation_list allocation_list;
size_t bytes_allocated = 0;

void exec(uint8_t* bytecode, struct vm_value* constants) {
  initialize_allocation_list(&allocation_list, 256);
  return run(bytecode, constants);
}

int main() {
  //struct tokenizer tok;
  struct compiler c;

  struct ast_node* ast = create_sample_ast();


  initialize_compiler(&c);
  compiler_gen(&c, ast);

  uint8_t* bytecode = c.bytecode;
  exec(bytecode, c.constants);

  compiler_emit(&c, OP_HALT);
  printf("Done compilation\n");


  printf("Bytes allocated: %zu\n", bytes_allocated);
  allocation_list_objects_free(&allocation_list);
  printf("Bytes allocated: %zu\n", bytes_allocated);


  /*
  char program[100] = "\"asd\"";
  initialize_tokenizer(&tok, program);
  tokenizer_get_next_token(&tok);

  printf("hello world");
  printf("hello world");


  */

  return 0;
}
