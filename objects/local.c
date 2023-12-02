#include "local.h"
#include "allocation.h"


void initialize_locals(struct locals* locals) {
  locals->count = 0;
}

/**
 * New local variable
 */
struct local* new_local(char* name, size_t scope_level) {
  struct local* l = (struct local*)allocate_heap(sizeof(struct local));
  strcpy(l->name, name);
  //g->name = name;
  l->scope_level = scope_level;

  return l;
}

/**
 * Adds a local struct to locals array.
 * returns the index
 */
int add_local(struct locals* locals_arr, char* name, size_t scope_level) {
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
  
  struct local* n = new_local(name, scope_level);
  int index = locals_arr->count;
  locals_arr->locals[index] = *n;

  locals_arr->count++;

  return index;
}

// @todo
// fix to accept globals struct instead of 
// array of globals
int get_local_index(struct locals* locals_arr, const char* name) {
  for (int i = 0; i < MAX_LOCAL_ARRAY_SIZE; i++) {
    if(strcmp(locals_arr->locals[i].name, name) == 0) {
      return i;
    }
  }

  return -1;
}


// this function is only used from inside the vm evaluation loop
// @todo
// check if index exists
void set_local(struct locals* locals_arr, int index, size_t scope_level) {

  locals_arr->locals[index].scope_level = scope_level;
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

