#include "ast.h"
#include <stdlib.h>

struct ast_node*  new_ast_node(struct ast_node ast) {
  struct ast_node *ptr = malloc(sizeof(struct ast_node));
  if (ptr) *ptr = ast;
  return ptr;
}

// creates a number
struct ast_node* create_sample_ast() {
  struct ast_node* term1 = AST_NEW(Program,
    AST_NEW(BinaryExpression,
      ADD, 
      AST_NEW(NumericLiteral, 4),
      AST_NEW(BinaryExpression,
        ADD,
        AST_NEW(NumericLiteral,2),
        AST_NEW(BinaryExpression,
          ADD,
          AST_NEW(NumericLiteral,7),
          AST_NEW(NumericLiteral,9),  // this value gets dealloced
        )
      )
    )
  );

  struct ast_node* term_ = AST_NEW(Program,
    AST_NEW(IfStatement,
      AST_NEW(BinaryExpression,
        GREATER_THAN,
        AST_NEW(NumericLiteral,4),
        AST_NEW(NumericLiteral, 2),
      ),
      AST_NEW(NumericLiteral, 5),   // this value gets dealloced
      AST_NEW(NumericLiteral, 6),   // this value gets dealloced
    )
  );


  return term1;
}
