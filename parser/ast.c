#include "ast.h"
#include <stdlib.h>


struct ast_node* new_ast_node(struct ast_node ast) {
  struct ast_node *ptr = malloc(sizeof(struct ast_node));
  if (ptr) *ptr = ast;
  return ptr;
}


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

      AST_NEW(NumericLiteral, 5),   // this value gets dealloced
      AST_NEW(NumericLiteral, 6),   // this value gets dealloced
    )
  );

  var_declar->next = compar;

  struct ast_node* bs = malloc(sizeof(struct ast_node));
  bs->type = BlockStatement;
  bs->next = NULL;
  bs->BlockStatement.body = var_declar;

  return bs;
}

// creates a number
struct ast_node* create_sample_ast() {

  struct ast_node* _term3 = ast_construct_body_statement();

  struct ast_node* term3 = AST_NEW(Program,
    _term3,
  );

  return term3;
}
