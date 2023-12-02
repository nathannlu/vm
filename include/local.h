#ifndef LOCAL_H
#define LOCAL_H 

#include <stdio.h>
#include <string.h>

#define MAX_LOCAL_ARRAY_SIZE 50
#define MAX_LOCAL_NAME_SIZE 10

// Definition of a global variable
struct local {
  char name[MAX_LOCAL_NAME_SIZE];
  size_t scope_level;
};

// Struct for hosting a list of global variables
struct locals {
  struct local    locals[MAX_LOCAL_ARRAY_SIZE];
  int             count;
};


struct local* new_local(char* name, size_t scope_level);

void initialize_locals(struct locals* locals);
int add_local(struct locals* locals_arr, char* name, size_t scope_level);
int get_local_index(struct locals* locals_arr, const char* name);
void set_local(struct locals* locals_arr, int index, size_t scope_level);

// @deprecated
//struct global* ALLOC_GLOBALS(struct global* values);

#endif
