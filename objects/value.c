/**
 * VM values
 */
#include "value.h"

void initialize_constants(struct constants* constants) {
  constants->count = 0;
}

int add_constant(struct constants* constants_arr, struct vm_value* value) {
  int index = constants_arr->count;
  constants_arr->constants[index] = *value;

  constants_arr->count++;

  return index;
}


// @todo move this to object.h
/*
struct vm_value ALLOC_NATIVE_FUNC(void* handler) {
  // create base object
  size_t obj_size = sizeof(struct object);
  struct object *obj = (struct object*)allocate_heap(obj_size);
  obj->type = FUNCTION;
  obj->data = handler;

  struct vm_value result;
  result.type = OBJECT;
  result.object = obj;  // this is the same for some reason

  return result;
}
*/

// @todo move this to constants.h 
/**
 * Allocate space on the heap for constants array
 */
/*
struct vm_value* ALLOC_CONSTANTS(struct vm_value* values) {
  size_t size = sizeof(values[0]) / sizeof(values);
  struct vm_value* array = (struct vm_value*)allocate_heap(size * sizeof(struct vm_value));

  // Copy values to the dynamic array
  for (size_t i = 0; i < size; ++i) {
    array[i] = values[i];
  }

  return array;
}
*/
