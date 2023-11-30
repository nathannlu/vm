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
  VariableDeclaration,
  AssignmentExpression,
  BinaryExpression,
  Identifier,
};

enum assignment_expression_map {
  SIMPLE_ASSIGN,    // e.g. x = 42
  ADDITION_ASSIGN,    // e.g. x += 42
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
  NOT_EQUAL,
};

struct Program {
  struct ast_node* body;
};
struct NumericLiteral {
  int number;
};
struct Identifier {
  char* name;
};
struct BinaryExpression {
  enum binary_expression_map  op;
  struct ast_node*            left;
  struct ast_node*            right;
};
struct VariableDeclaration {
  struct ast_node* id;     // this has to be identifier struct
  struct ast_node* init;
};
struct AssignmentExpression {
  enum assignment_expression_map op;
  struct ast_node*            left;     // this has to be identifier struct
  struct ast_node*            right;
};
struct IfStatement {
  struct ast_node* test;
  struct ast_node* consequent;
  struct ast_node* alternate;
};
struct ast_node {
  enum ast_node_type  type;
  struct ast_node*    next;
  union {
    struct Program              Program;
    struct Identifier           Identifier;
    struct NumericLiteral       NumericLiteral;
    struct BinaryExpression     BinaryExpression;
    struct AssignmentExpression AssignmentExpression;
    struct IfStatement          IfStatement;
    struct VariableDeclaration  VariableDeclaration;
  };
};

struct ast_node*  new_ast_node(struct ast_node ast);

// creates a number
struct ast_node* create_sample_ast();


#define AST_NEW(type, ...) \
  new_ast_node((struct ast_node){type, NULL, {.type=(struct type){__VA_ARGS__}}})


#endif
