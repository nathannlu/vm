#include "ast.h"
#include <stdlib.h>


struct ast_node* new_ast_node(struct ast_node ast) {
  struct ast_node *ptr = malloc(sizeof(struct ast_node));
  if (ptr) *ptr = ast;
  return ptr;
}


/*
struct ast_node* ast_construct_body_statement() {
  struct ast_node* var_declar = AST_NEW(Program,
    AST_NEW(VariableDeclaration,
      AST_NEW(Identifier, "x"),
      AST_NEW(NumericLiteral, 2),
    )
  );
  struct ast_node* compar = AST_NEW(Program,
    AST_NEW(IfStatement,
      AST_NEW(BinaryExpression,
        GREATER_THAN,
        AST_NEW(Identifier, "x"),
        AST_NEW(NumericLiteral, 2),
      ),
      AST_NEW(BinaryExpression,
        ADD,
        AST_NEW(NumericLiteral,7),
        AST_NEW(NumericLiteral,9),  // this value gets dealloced
      ),
      AST_NEW(BinaryExpression,
        SUB,
        AST_NEW(NumericLiteral,7),
        AST_NEW(NumericLiteral,9),  // this value gets dealloced
      ),
    )
  );

  var_declar->next = compar;

  struct ast_node* bs = malloc(sizeof(struct ast_node));
  bs->type = BlockStatement;
  bs->next = NULL;
  bs->BlockStatement.body = var_declar;

  return bs;
}
*/

