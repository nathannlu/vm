#ifndef GLOBAL_H
#define GLOBAL_H

#include "value.h"
#include <stdio.h>
#include <string.h>

#define MAX_GLOBAL_ARRAY_SIZE 50
#define MAX_GLOBAL_NAME_SIZE 10

struct global {
  char name[MAX_GLOBAL_NAME_SIZE];
  struct vm_value* value;
};

struct globals {
  struct global   globals[MAX_GLOBAL_ARRAY_SIZE];
  int             count;
};


void initialize_globals(struct globals* globals);
int define_global(struct globals* globals, char* name);
struct global* new_global(char* name);
int get_global_index(struct globals* globals_arr, const char* name);

// deprecated funcs
struct global* ALLOC_GLOBALS(struct global* values);
void set_global(struct global globals[], int index, struct vm_value* value);

#endif
