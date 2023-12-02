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
    printf("Consume_token error: expected type %d but got %d\n", type, current.type);
  }

  lookahead_token = get_next_token();

  return current;
}

static struct ast_node* parse_numeric_literal() {
  int number = consume_token(INTEGER).value;
  return AST_NEW(NumericLiteral, number);
}

static struct ast_node* parse_identifier() {
  char* name = consume_token(ID).string;
  return AST_NEW(Identifier, name);
}

static struct ast_node* parse_literal() {
  switch (lookahead_token.type) {
    case INTEGER:
      return parse_numeric_literal();
    case ID:
      return parse_identifier();

    // @todo strings
  }
}


// If statement looks like...
// 'if' '(' 'Expression' ')' Statement 'else' Statement
static struct ast_node* parse_if_statement() {

  consume_token(IF);
  consume_token(LPAREN);

  struct ast_node* test = parse_expression();

  consume_token(RPAREN);

  struct ast_node* consequent = parse_statement();
  consume_token(SEMICOLON);
  
  struct ast_node* alternate;

  // Check lookahead to see if it 
  // contains 'else' keyword
  if(true) {
    consume_token(ELSE);

    alternate = parse_statement();
    consume_token(SEMICOLON);
  } else {
    alternate = NULL;

  }

  return AST_NEW(IfStatement,
    test,
    consequent,
    alternate,
  );
}

// def x = 123;
static struct ast_node* parse_variable_declaration() {
  consume_token(DEF);

  struct ast_node* id = parse_identifier();    // AST_NEW(Identifier, "x")


  // check if next token is 
  // ; or =
  struct ast_node* init;
  if (lookahead_token.type == SEMICOLON) {
    init = NULL;
  } else if (lookahead_token.type == ASSIGN) {
    consume_token(ASSIGN);

    init = parse_statement();
  }

  // why can i not consume a semicolon here?
  // for some reason if i consume semicolon it 
  // offsets the whole code.
  //consume_token(SEMICOLON);

  return AST_NEW(VariableDeclaration,
    id,
    init
  );
}

static struct ast_node* parse_statement_list(enum token_type until) {
  struct ast_node* statement_head = parse_statement();
  struct ast_node* current_statement = statement_head;

  consume_token(SEMICOLON);
  
  while(lookahead_token.type != until) {
    struct ast_node* new_statement = parse_statement();
    consume_token(SEMICOLON);

    current_statement->next = new_statement;
    current_statement = new_statement;
  }

  return statement_head;
}

static struct ast_node* parse_block_statement() {
  consume_token(LBRACE);
  struct ast_node* body = parse_statement_list(RBRACE);
  consume_token(RBRACE);

  struct ast_node* block = AST_NEW(BlockStatement, body);
  return block;
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
    case ID:
      return parse_expression();

    case LBRACE:
      return parse_block_statement();

    case IF:
      return parse_if_statement();

    case DEF:
      return parse_variable_declaration();
  }
}

struct ast_node* parse_program() {
  struct ast_node* body = parse_statement_list(END_OF_INPUT);
  struct ast_node* program = AST_NEW(Program, body);
  return program;
}
