#ifndef GLOBAL_H
#define GLOBAL_H

#include "value.h"
#include <stdio.h>
#include <string.h>

#define MAX_ARRAY_SIZE 10
#define MAX_GLOBAL_NAME_SIZE 10

struct global {
  char name[MAX_GLOBAL_NAME_SIZE];
  struct vm_value* value;
};

/**
 * Allocate space on the heap for globals array
 */
struct global* ALLOC_GLOBALS(struct global* values);

/**
 * Adds a global struct to globals array
 */
void define_global(struct global globals[], char* name);
void set_global(struct global globals[], int index, struct vm_value* value);
int get_global_index(struct global globals[], const char* name);

#endif
