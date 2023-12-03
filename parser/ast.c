#include "ast.h"
#include <stdlib.h>


struct ast_node* new_ast_node(struct ast_node ast) {
  struct ast_node *ptr = malloc(sizeof(struct ast_node));
  if (ptr) *ptr = ast;
  return ptr;
}
