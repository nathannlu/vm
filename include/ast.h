#ifndef AST_H 
#define AST_H

enum ast_node_type {
  Program,
  ExpressionStatement,
  IfStatement,
  BlockStatement,
  NumericLiteral,
  StringLiteral, 
  FunctionDeclaration,
  BinaryExpression,
  Identifier,
};

enum binary_expression_map {
  ADD,
  SUB,
  MUL,
  DIV,

  LESS_THAN,
  GREATER_THAN,
  EQUAL,
  GREATER_EQUAL_THAN,
  LESS_EQUAL_THAN,
  NOT_EQUAL
};

struct Program {
  struct ast_node* body;
};
struct NumericLiteral {
  int number;
};
struct BinaryExpression {
  enum binary_expression_map  op;
  struct ast_node*            left;
  struct ast_node*            right;
};
struct IfStatement {
  struct ast_node* test;
  struct ast_node* consequent;
  struct ast_node* alternate;
};
struct ast_node {
  enum ast_node_type  type;
  union {
    struct Program          Program;
    struct NumericLiteral   NumericLiteral;
    struct BinaryExpression BinaryExpression;
    struct IfStatement      IfStatement;
  };
};

struct ast_node*  new_ast_node(struct ast_node ast);

// creates a number
struct ast_node* create_sample_ast();

#define AST_NEW(type, ...) \
  new_ast_node((struct ast_node){type, {.type=(struct type){__VA_ARGS__}}})


#endif
