#ifndef TOKENS_H 
#define TOKENS_H

// Token types
enum token_type {
  DEF,
  IF,
  ELSE,
  INTEGER,
  SEMICOLON,
  ASSIGN,
  ID,
  FUNC,
  COMMA,
  PLUS,
  MINUS,
  MULTIPLY,
  DIVIDE,
  GREATER,
  LESS,
  GREATEREQUAL,
  LESSEQUAL,
  LPAREN,
  RPAREN,
  LBRACE,
  RBRACE,
  END_OF_INPUT
};

// Token structure
struct token {
  enum token_type   type;
  int               value; // For INTEGER tokens
  char*             string; // For STRING and ID tokens
};


#endif
