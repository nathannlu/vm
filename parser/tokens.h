#ifndef TOKENS_H 
#define TOKENS_H

// Token types
enum token_type {
  DEF,
  IF,
  ELSE,
  INTEGER,
  SEMICOLON,
  
  PLUS,
  MINUS,
  MULTIPLY,
  DIVIDE,
  LPAREN,
  RPAREN,
  END_OF_INPUT
};

// Token structure
struct token {
  enum token_type type;
  int value; // For INTEGER tokens
};


#endif
