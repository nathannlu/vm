/**
 * VM values
 */
#ifndef VM_VALUES
#define VM_VALUES

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "object.h"
//#include "heap/allocation.c"

enum vm_value_type {
  NUMBER,
  BOOLEAN,
  OBJECT,
};

struct vm_value {
  enum vm_value_type  type;
  int                 scope;
  union {
    double          number;
    struct object*  object;   // Objects will be stored on the heap. So store the pointer in vm_value 
    bool            boolean;
  };
};

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

#define NUMBER(value, ...) ((struct vm_value){NUMBER, ##__VA_ARGS__, .number = value})

#define AS_NUMBER(vm_value) ((double)vm_value.number)

#define BOOLEAN(value) ((struct vm_value){BOOLEAN, .boolean = value})
#define AS_BOOLEAN(vm_value) ((bool)vm_value.boolean)

#define AS_STRING(vm_value) ((char*)vm_value.object->data)

// returns a vm_value
struct vm_value ALLOC_OBJECT(void *value) {
  // create base object
  size_t obj_size = sizeof(struct object);
  struct object *obj = (struct object*)allocate_heap(obj_size);
  obj->type = FUNCTION;
  obj->data = value;

  struct vm_value result;
  result.type = OBJECT;
  result.object = obj;  // this is the same for some reason

  return result;
}

// returns a vm_value for string
struct vm_value ALLOC_STRING(const char *value) {

  // create string object
  size_t str_size = strlen(value) + 1;
  void *str_obj = (void*)strdup(value);
  track_allocation(str_obj, str_size);


  // create base object
  size_t obj_size = sizeof(struct object);
  struct object *obj = (struct object*)allocate_heap(obj_size);
  obj->type = STRING;
  obj->data = str_obj;

  // add trace whenever there is a malloc
  bytes_allocated += obj_size;
  bytes_allocated += str_size;

  //list_push_tail(&objects, &obj->trace);

  struct vm_value result;
  result.type = OBJECT;
  result.object = obj;  // this is the same for some reason

  return result;
}

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

/**
 * Allocate space on the heap for globals array
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

#endif 
