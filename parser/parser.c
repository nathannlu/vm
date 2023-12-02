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

  lookahead_token = get_next_token();

  return current;
}


// If statement looks like...
// 'if' '(' 'Expression' ')' Statement 'else' Statement
/*
struct ast_node* parse_if_statement() {
  printf("hello");

  consume_token(IF);
  consume_token(LPAREN);

  struct ast_node* test = parse_expression();

  consume_token(RPAREN);

  struct ast_node* consequent = parse_statement();

  // Check lookahead to see if it 
  // contains 'else' keyword
  struct ast_node* alternate;
  if(true) { 
    consume_token(ELSE);
    alternate = parse_statement();
  } else {
    alternate = NULL;
  }

  return AST_NEW(IfStatement,
    test,
    consequent,
    alternate,
  );
}
*/

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


// Function to parse a statement
struct ast_node* parse_statement() {

  return parse_literal();
  
  /*
  switch (lookahead_token.type) {
    case DEF:
      return parse_variable_statement();
    case LPAREN:
      return parse_block_statement();
    case IF:
      return parse_if_statement();
    default:
      printf("Unexpected token");
  }
    */
}




struct ast_node* parse_program() {
  struct ast_node* body = parse_statement_list();
  struct ast_node* var_declar = AST_NEW(Program, body);
}
