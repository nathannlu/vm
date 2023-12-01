#ifndef GLOBAL_H
#define GLOBAL_H

#include "value.h"
#include <stdio.h>
#include <string.h>

#define MAX_GLOBAL_ARRAY_SIZE 50
#define MAX_GLOBAL_NAME_SIZE 10

// Definition of a global variable
struct global {
  char name[MAX_GLOBAL_NAME_SIZE];
  struct vm_value value;
};

// Struct for hosting a list of global variables
struct globals {
  struct global   globals[MAX_GLOBAL_ARRAY_SIZE];
  int             count;
};


struct global* new_global(char* name);

void initialize_globals(struct globals* globals);
int define_global(struct globals* globals, char* name);
int get_global_index(struct globals* globals_arr, const char* name);
void set_global(struct globals* globals_arr, int index, struct vm_value value);

// @deprecated
struct global* ALLOC_GLOBALS(struct global* values);
struct globals* clone_globals(const struct globals* source);

#endif
