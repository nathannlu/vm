#ifndef EVAL_H
#define EVAL_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "codeobject.h"
#include "stack.h"
#include "callstack.h"


struct eval {
  uint8_t *ip;
  struct code_object* co;
  struct Stack stack;
  struct callstack cstack;
};

extern uint8_t *ip; // instruction pointer - always points to the next instruction

uint8_t read_byte();
uint16_t read_short();
void run(struct code_object* co);

#endif
