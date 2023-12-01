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

void exec(uint8_t* bytecode, struct vm_value* constants, struct globals* globals) {
  initialize_allocation_list(&allocation_list, 256);
  return run(bytecode, constants, globals);
}

int main() {
  // allocate global object in heap
  struct globals globals;       // init globals struct
  initialize_globals(&globals);

  struct vm_value constants[MAX_GLOBAL_ARRAY_SIZE];

  //struct tokenizer tok;
  struct compiler c;
  initialize_compiler(&c, constants, &globals);

  struct ast_node* ast = create_sample_ast();

  compiler_gen(&c, ast);

  // manual testing bytecode
  //compiler_emit(&c, OP_GET_GLOBAL);
  //compiler_emit(&c, 0);

  // ending
  compiler_emit(&c, OP_HALT);
  printf("Done compilation\n");

  // Run bytecode
  uint8_t* bytecode = c.bytecode;
  exec(bytecode, constants, &globals);

  printf("Bytes allocated: %zu\n", bytes_allocated);
  allocation_list_objects_free(&allocation_list);
  printf("Bytes allocated: %zu\n", bytes_allocated);


  /*
  char program[100] = "\"asd\"";
  initialize_tokenizer(&tok, program);
  tokenizer_get_next_token(&tok);

  */

  return 0;
}
