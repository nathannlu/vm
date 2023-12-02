#include "global.h"


void initialize_globals(struct globals* globals) {
  globals->count = 0;
}

/**
 * Initialize new globals to 0
 */
struct global* new_global(char* name) {
  struct global* g = (struct global*)allocate_heap(sizeof(struct global));
  strcpy(g->name, name);
  //g->name = name;
  g->value = NUMBER(0.0);

  return g;
}

/**
 * Adds a global struct to globals array.
 * returns the index
 */
int define_global(struct globals* globals_arr, char* name) {
  // check if global is already defined
  // this errors out right now because it
  // does a strcmp against mem blocks w null
  // values
  /*
  int index = get_global_index(globals_arr->globals, name);

  // already defined
  if (index != -1) {
    return index;
  }
  */
  
  struct global* n = new_global(name);
  int index = globals_arr->count;
  globals_arr->globals[index] = *n;

  globals_arr->count++;

  return index;
}

// @todo
// fix to accept globals struct instead of 
// array of globals
int get_global_index(struct globals* globals_arr, const char* name) {
  for (int i = 0; i < MAX_GLOBAL_ARRAY_SIZE; i++) {
    if(strcmp(globals_arr->globals[i].name, name) == 0) {
      return i;
    }
  }

  return -1;
}


// this function is only used from inside the vm evaluation loop
// @todo
// check if index exists
void set_global(struct globals* globals_arr, int index, struct vm_value value) {

  globals_arr->globals[index].value = value;
  return;
}


/**
 * @WILL BE DEPRECATED SOON
 * Allocate space on the heap for globals array
 */
/*
struct global* ALLOC_GLOBALS(struct global* values) {
  size_t size = sizeof(values[0]) / sizeof(values);
  struct global* array = (struct global*)allocate_heap(size * sizeof(struct global));

  // Copy values to the dynamic array
  for (size_t i = 0; i < size; ++i) {
    array[i] = values[i];
  }

  return array;
}
*/

