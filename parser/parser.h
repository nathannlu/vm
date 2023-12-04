#ifndef PARSER_H
#define PARSER_H

#include "tokens.h"

// Function to consume the next token
void initialize_parser();
struct token consume_token(enum token_type type);
struct ast_node* parse_program();
struct ast_node* parse_statement();
struct ast_node* parse_expression();
struct ast_node* parse_literal();
struct ast_node* parse_identifier();



#endif
