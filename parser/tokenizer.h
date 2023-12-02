#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "tokens.h"


// Function to initialize the tokenizer with input
void init_tokenizer(char* input);

// Function to get the next token from the input
struct token get_next_token();

#endif /* TOKENIZER_H */

