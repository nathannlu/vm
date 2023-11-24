#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <string.h>
#include "value.h"

#define MAX_ARRAY_SIZE 10
#define MAX_GLOBAL_NAME_SIZE 10

struct global {
  char name[MAX_GLOBAL_NAME_SIZE];
  struct vm_value value;
};

int get_global_index(struct global globals[], const char* name) {
  for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
    if(strcmp(globals[i].name, name) == 0) {
      return i;
    }
  }

  return -1;
}

/**
 * Allocate space on the heap for globals array
 */
struct global* ALLOC_GLOBALS(struct global* values) {
  size_t size = sizeof(values[0]) / sizeof(values);
  struct global* array = (struct global*)allocate_heap(size * sizeof(struct global));

  // Copy values to the dynamic array
  for (size_t i = 0; i < size; ++i) {
    array[i] = values[i];
  }

  return array;
}

/**
 * Adds a global struct to globals array
 */
void define_global(struct global globals[], char* name) {
  // check if global is already defined
  int index = get_global_index(globals, name);

  // already defined
  if (index != -1) {
    return;
  }

  // @todo
  // add to globals array
}

void set_global(struct global globals[], int index, struct vm_value value) {
  // @todo
  // check if index exists

  globals[index].value = value;
  return;
}

#endif
