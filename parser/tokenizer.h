#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <ctype.h>
#include <string.h>
#include "ast.h"

#define MAX_PROGRAM_SIZE 1000

#define MAX_TOKEN_LENGTH 20

// Responsible for reading a buffer of
// bytes and returning the 
struct tokenizer {
  char* buffer;     // Input buffer, or NULL; malloc'ed if fp != NULL 
  char* cursor;     // Next character in the buffer
};

struct tokenizer_output {
  enum ast_node_type  type;
  void*               value;
};


void initialize_tokenizer(struct tokenizer* tok, char* buffer) {
  tok->buffer = buffer;
  tok->cursor = &tok->buffer[0];
}

void tokenizer_get_next_token(struct tokenizer* tok) {

  char buf_value = *tok->cursor;
  printf("The character is: %c\n", buf_value);

  char value[MAX_TOKEN_LENGTH];
  int i = 0;

  // Handle numbers
  if(isdigit(buf_value)) {

    // keep looping if its a number
    while(isdigit(*tok->cursor)) {
      value[i] = *tok->cursor;
      i++;

      // Read next value
      tok->cursor++;
    }

    // Null-terminate the string
    value[i] = '\0';

    int int_value = atoi(value);
    printf("Parsed integer value: %d\n", int_value);

    return;
  }

  // Handle strings
  if(buf_value == '"') {
    tok->cursor++;

    // keep looping until you
    // reach the end quotation
    while(*tok->cursor != '"') {
      value[i] = *tok->cursor;
      i++;

      // Read next value
      tok->cursor++;
    }

    printf("Parsed string value: %.5s\n", value);

    return;
  }

  tok->cursor++;
}

#endif
