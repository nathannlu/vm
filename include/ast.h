#ifndef AST_H 
#define AST_H

/**
 * Implementation of the AST tree.
 * AST tree are comprised of linked list nodes,
 * each node has a pointer to the next node to
 * represent a list of children. The tree is
 * traversed downwards through its union of
 * ast_node_type named structs.
 *
 * AST tree design follows https://astexplorer.net/
 */

enum assignment_expression_map { SIMPLE_ASSIGN, ADDITION_ASSIGN, 
                                 SUBTRACTION_ASSIGN, MULTIPLICATIVE_ASSIGN,
                                 DIVISION_ASSIGN };

enum binary_expression_map { ADD, SUB, MUL, DIV, LESS_THAN, 
                             GREATER_THAN, EQUAL, GREATER_EQUAL_THAN,
                             LESS_EQUAL_THAN, NOT_EQUAL };

// Enum ast_node_type must have the same-named structs.
// The AST_NEW macro creates nodes using the ast_node_type
// as a parameter, and creates structs with the matching
// name.
enum ast_node_type { Program, Identifier, NumericLiteral,
                     StringLiteral, FunctionDeclaration, 
                     VariableDeclaration, IfStatement, BlockStatement, 
                     AssignmentExpression, BinaryExpression, 
                     CallExpression };

// Different types of nodes in the AST tree
struct Program {
  struct ast_node* body;
};
struct NumericLiteral {
  int number;
};
struct Identifier {
  char* name;
};
struct VariableDeclaration {
  struct ast_node* id;                  // This has to be struct Identifier
  struct ast_node* init;
};
struct FunctionDeclaration {
  struct ast_node* id;                  // This has to be struct Identifier
  struct ast_node* params;              // This is a linked list
  struct ast_node* body;     
};
struct CallExpression {
  struct ast_node* callee;              // This has to be struct Identifier
  struct ast_node* arguments;           // This is a linked list  
};
struct BinaryExpression {
  enum binary_expression_map  op;
  struct ast_node*            left;
  struct ast_node*            right;
};
struct AssignmentExpression {
  enum assignment_expression_map op;
  struct ast_node*            left;     // This has to be struct Identifier
  struct ast_node*            right;
};
struct BlockStatement {
  struct ast_node* body;
};
struct IfStatement {
  struct ast_node* test;
  struct ast_node* consequent;
  struct ast_node* alternate;
};

// This struct describes the general AST node
// If a node's parent describes a list for e.g. 
// its body, the node can create sibling nodes by
// linking them to the next property.
struct ast_node {
  enum ast_node_type  type;
  struct ast_node*    next;
  union {
    struct Program              Program;

    struct BlockStatement       BlockStatement;
    struct IfStatement          IfStatement;
    
    struct Identifier           Identifier;
    struct NumericLiteral       NumericLiteral;
    struct VariableDeclaration  VariableDeclaration;
    struct FunctionDeclaration  FunctionDeclaration;

    struct BinaryExpression     BinaryExpression;
    struct AssignmentExpression AssignmentExpression;
    struct CallExpression       CallExpression;
  };
};

struct ast_node*  new_ast_node(struct ast_node ast);

#define AST_NEW(type, ...) \
  new_ast_node((struct ast_node){type, NULL, {.type=(struct type){__VA_ARGS__}}})


#endif
