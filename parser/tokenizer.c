#include "tokenizer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Global variables
static char* input; // Pointer to the input string
static struct token current_token; // Current token being processed

// Function to advance to the next character in the input
static void advance() {
  input++;
}

// Function to skip whitespace in the input
static void skip_whitespace() {
  while (*input && isspace(*input)) {
    advance();
  }
}

// Function to parse an integer from the input
static int parse_integer() {
    int result = 0;
    while (*input && isdigit(*input)) {
        result = result * 10 + (*input - '0');
        advance();
    }
    return result;
}

// Function to tokenize one or more word characters
static char* parse_identifier() {
  // Allocate memory for the word
  char *word = NULL;
  int length = 0;

  // Tokenize word characters
  while (*input && isalnum(*input)) {
    word = realloc(word, length + 2); // +1 for the new character, +1 for null terminator
    word[length++] = *input;
    advance();
  }

  // Null-terminate the word
  if (word != NULL) {
      word[length] = '\0';
  }

  return word;
}


// Function to initialize the tokenizer with input
void init_tokenizer(char* input_str) {
    input = input_str;
}

// Function to get the next token from the input
struct token get_next_token() {
  skip_whitespace();

  if (*input == '\0') {
    return (struct token){END_OF_INPUT, 0};
  } else if (isdigit(*input)) {
    int num = 0;

    // Move to next characters
    // to parse the full integer
    num = parse_integer();

    return (struct token){INTEGER, num, NULL};

  } else if (*input == '"') {
    // @todo handle strings
    /*
    struct token tok;
    //tok.type = STRING;

    // Parse the string
    int i = 0;
    while (isalpha(*input)) {
      //tok.value.string[i++] = *input;
      input++;
    }
    //token.value.string[i] = '\0';
    */
  }

  else if (*input == '+') {
    advance();
    return (struct token){PLUS, 0, NULL};

  } else if (*input == '-') {
    advance();
    return (struct token){MINUS, 0, NULL};

  } else if (*input == '<') {
    advance();
    return (struct token){LESS, 0, NULL};

  } else if (*input == '>') {
    advance();
    return (struct token){GREATER, 0, NULL};

  } else if (*input == ';') {
    advance();
    return (struct token){SEMICOLON, 0, NULL};

  } else if (*input == '(') {
    advance();
    return (struct token){LPAREN, 0, NULL};

  } else if (*input == ')') {
    advance();
    return (struct token){RPAREN, 0, NULL};

  } else if (*input == '{') {
    advance();
    return (struct token){LBRACE, 0, NULL};

  } else if (*input == '}') {
    advance();
    return (struct token){RBRACE, 0, NULL};

  } else if (*input == '=') {
    advance();
    return (struct token){ASSIGN, 0, NULL};

  } else if (*input == ',') {
    advance();
    return (struct token){COMMA, 0, NULL};

  }  else if (strncmp(input, "if", 2) == 0 && !isalnum(input[2])) {
    advance();
    advance();
    return (struct token){IF, 0, NULL};

  } else if (strncmp(input, "else", 4) == 0 && !isalnum(input[4])) {
    advance();
    advance();
    advance();
    advance();
    return (struct token){ELSE, 0, NULL};

  } else if (strncmp(input, "def", 3) == 0 && !isalnum(input[3])) {
    advance();
    advance();
    advance();
    return (struct token){DEF, 0,NULL};
  } else if (strncmp(input, "func", 4) == 0 && !isalnum(input[4])) {
    advance();
    advance();
    advance();
    advance();
    return (struct token){FUNC, 0,NULL};
  }

  // Handle identifier.
  // This code must be last so it doesn't
  // count other symbols as identifier
  else if (isalpha(*input)) {
    char* word = parse_identifier();
    return (struct token){ID, 0, word};
  }
  else {
    // Handle other characters if needed
    printf("Unknown token");
    input++;
  }
}

