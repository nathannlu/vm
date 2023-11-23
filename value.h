/**
 * VM values
 */
#ifndef VM_VALUES
#define VM_VALUES

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "heap/allocation.c"

enum vm_value_type {
  NUMBER,
  BOOLEAN,
  OBJECT,
};


enum object_type {
  STRING,
  FUNCTION,
};

// different types of objects that
// are stored on the heap:
struct object {
  enum object_type type;
	//struct list_node trace;
  void *data;
};

struct vm_value {
  enum vm_value_type type;
  union {
    double          number;
    struct object*  object;   // Objects will be stored on the heap. So store the pointer in vm_value 
    bool            boolean;
  };
};

#define NUMBER(value) ((struct vm_value){NUMBER, .number = value})
#define AS_NUMBER(vm_value) ((double)vm_value.number)

#define BOOLEAN(value) ((struct vm_value){BOOLEAN, .boolean = value})
#define AS_BOOLEAN(vm_value) ((bool)vm_value.boolean)

#define AS_STRING(vm_value) ((char*)vm_value.object->data)

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

struct vm_value ALLOC_FUNC(void* handler) {
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

#endif 
