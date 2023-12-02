#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "value.h"
#include "codeobject.h"
#include "allocation.h"
#include "op_code.h"

#include "eval.h"

#include "tokenizer.h"
#include "compiler.h"
#include "ast.h"
#include "parser.h"


struct _allocation_list allocation_list;
size_t bytes_allocated = 0;


static struct token current_token; // Current token being processed

void exec(struct ast_node* ast) {
  initialize_allocation_list(&allocation_list, 256);

  // allocate global object in heap
  struct globals globals;       // init globals struct
  initialize_globals(&globals);

  struct vm_value constants[MAX_GLOBAL_ARRAY_SIZE];

  //struct tokenizer tok;
  struct compiler c;
  initialize_compiler(&c, constants, &globals);

  //struct ast_node* ast = create_sample_ast();

  compiler_gen(&c, ast);

  // manual testing bytecode
  //compiler_emit(&c, OP_GET_GLOBAL);
  //compiler_emit(&c, 0);

  // ending
  compiler_emit(&c, OP_HALT);
  printf("Done compilation\n");

  // Run bytecode
  uint8_t* bytecode = c.bytecode;

  run(bytecode, constants, &globals);

  printf("Bytes allocated: %zu\n", bytes_allocated);
  allocation_list_objects_free(&allocation_list);
  printf("Bytes allocated: %zu\n", bytes_allocated);

  return;
}


int main(int argc, char *argv[]) {

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    return 1; // Return with an error code
  }

  // Open the file for reading
  FILE *file = fopen(argv[1], "r");
  if (file == NULL) {
    perror("Error opening file");
    return 1; // Return with an error code
  }

  char buffer[256];
  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    //char input_str[] = "if (3 < 4) 3; else 6;";
    init_tokenizer(buffer);

    // lookahead_token needs to be initialized to
    // the first token
    initialize_parser();
    struct ast_node* program = parse_program();

    exec(program);
  }

  // Close the file
  fclose(file);

  return 0;
}
