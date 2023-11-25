#include "local.h"

int get_local_index(struct local locals[], const char* name) {
  for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
    if(strcmp(locals[i].name, name) == 0) {
      return i;
    }
  }

  return -1;
}

/**
 * Allocate space on the heap for globals array
 */
struct local* ALLOC_LOCALS(struct local* values) {
  size_t size = sizeof(values[0]) / sizeof(values);
  struct local* array = (struct local*)allocate_heap(size * sizeof(struct local));

  // Copy values to the dynamic array
  for (size_t i = 0; i < size; ++i) {
    array[i] = values[i];
  }

  return array;
}

void set_local(struct local locals[], int index, int stack_index) {
  // @todo
  // check if index exists

  locals[index].stack_index = stack_index;
  return;
}

