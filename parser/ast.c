#include "ast.h"
#include <stdlib.h>

struct ast_node*  new_ast_node(struct ast_node ast) {
  struct ast_node *ptr = malloc(sizeof(struct ast_node));
  if (ptr) *ptr = ast;
  return ptr;
}

// creates a number
struct ast_node* create_sample_ast() {
  struct ast_node* term_ = AST_NEW(Program,
    AST_NEW(BinaryExpression,
      MUL, 
      AST_NEW(NumericLiteral, 2),
      AST_NEW(NumericLiteral,4)
    )
  );

  return term_;
}
