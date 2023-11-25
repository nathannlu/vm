#ifndef LOCAL_H
#define LOCAL_H

#include <stdio.h>
#include <string.h>
#include "value.h"

#define MAX_ARRAY_SIZE 10
#define MAX_LOCAL_NAME_SIZE 10

struct local {
  char name[MAX_LOCAL_NAME_SIZE];
  int stack_index;
  //struct vm_value value;
};

int get_local_index(struct local locals[], const char* name);
struct local* ALLOC_LOCALS(struct local* values);
void set_local(struct local locals[], int index, int stack_index);

#endif
