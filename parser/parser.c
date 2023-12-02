#include "parser.h"
#include "tokenizer.h" 
#include "ast.h"
#include <stdio.h>
#include <stdbool.h>


struct token lookahead_token;


void initialize_parser() {
  lookahead_token = get_next_token();
}

// Expects a token of a given type.
// Advances to next token.
struct token consume_token(enum token_type type) {
  struct token current = lookahead_token;

  // @todo
  // verify the type
  if(current.type != type) {
    printf("Consume_token error: mismatch types %d\n", type);
  }

  lookahead_token = get_next_token();

  return current;
}

struct ast_node* parse_numeric_literal() {
  struct token tok =  consume_token(INTEGER);
  return AST_NEW(NumericLiteral, tok.value);
}

struct ast_node* parse_literal() {
  switch (lookahead_token.type) {
    case INTEGER:
      return parse_numeric_literal();
  }
}

// If statement looks like...
// 'if' '(' 'Expression' ')' Statement 'else' Statement
struct ast_node* parse_if_statement() {

  consume_token(IF);
  consume_token(LPAREN);

  struct ast_node* test = parse_expression();

  consume_token(RPAREN);

  struct ast_node* consequent = parse_statement();
  consume_token(SEMICOLON);

  // Check lookahead to see if it 
  // contains 'else' keyword
  consume_token(ELSE);

  struct ast_node* alternate = parse_statement();
  //struct ast_node* alternate = AST_NEW(NumericLiteral, 9);
  consume_token(SEMICOLON);

  // @todo
  // support no 'else'
  /*
  if(true) { 
    consume_token(ELSE);
    alternate = parse_statement();
  } else {
    alternate = NULL;
  }
  */

  return AST_NEW(IfStatement,
    test,
    consequent,
    alternate,
  );
}



struct ast_node* parse_statement_list() {
  struct ast_node* statement_head = parse_statement();
  struct ast_node* current_statement = statement_head;

  consume_token(SEMICOLON);
  
  while(lookahead_token.type != END_OF_INPUT) {
    struct ast_node* new_statement = parse_statement();
    consume_token(SEMICOLON);

    current_statement->next = new_statement;
    current_statement = new_statement;
  }

  return statement_head;
}


// handles all binary expressions
struct ast_node* parse_expression() {
  struct ast_node* left = parse_literal();

  while(lookahead_token.type == PLUS || lookahead_token.type == MINUS || lookahead_token.type == GREATER || lookahead_token.type == LESS) { 
    if(lookahead_token.type == PLUS) {
      struct token operator = consume_token(PLUS);
      struct ast_node* right = parse_literal();

      left = AST_NEW(BinaryExpression,
        ADD,
        left,
        right
      );
    } else if (lookahead_token.type == MINUS) {
      struct token operator = consume_token(MINUS);
      struct ast_node* right = parse_literal();

      left = AST_NEW(BinaryExpression,
        SUB,
        left,
        right
      );
    } else if (lookahead_token.type == LESS) {
      struct token operator = consume_token(LESS);
      struct ast_node* right = parse_literal();

      left = AST_NEW(BinaryExpression,
        LESS_THAN,
        left,
        right
      );
    } else if (lookahead_token.type == GREATER) {
      struct token operator = consume_token(GREATER);
      struct ast_node* right = parse_literal();

      left = AST_NEW(BinaryExpression,
        GREATER_THAN,
        left,
        right
      );
    }
  }

  return left;
}

// Function to parse a statement
struct ast_node* parse_statement() {
  switch (lookahead_token.type) {

    // if next token is a number, render expression
    //    if next token is an operation, render binary operator
    // handles operations such as:
    // 3 + 4;
    // 4 < 5;
    case INTEGER:
      return parse_expression();

    case IF:
      return parse_if_statement();
  }
}

struct ast_node* parse_program() {
  struct ast_node* body = parse_statement_list();
  struct ast_node* var_declar = AST_NEW(Program, body);
}
