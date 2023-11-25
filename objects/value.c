/**
 * VM values
 */
#include "value.h"


// Function to compare two vm_value instances
bool vm_value_cmp(struct vm_value value1, struct vm_value value2) {
  if (value1.type != value2.type) {
    return false;  // Different types, not equal
  }
  switch (value1.type) {
    case NUMBER:
      return value1.number == value2.number;
    case OBJECT:
      // Compare pointers or object contents depending on your needs
      return value1.object == value2.object;
    case BOOLEAN:
      return value1.boolean == value2.boolean;
    default:
      return false;  // Unknown type, consider them not equal
  }
}

// @todo move this to object.h
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

// @todo move this to constants.h 
/**
 * Allocate space on the heap for constants array
 */
struct vm_value* ALLOC_CONSTANTS(struct vm_value* values) {
  size_t size = sizeof(values[0]) / sizeof(values);
  struct vm_value* array = (struct vm_value*)allocate_heap(size * sizeof(struct vm_value));

  // Copy values to the dynamic array
  for (size_t i = 0; i < size; ++i) {
    array[i] = values[i];
  }

  return array;
}

