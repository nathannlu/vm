/**
 * VM values
 */
#ifndef VM_VALUES
#define VM_VALUES

#include <stdio.h>
#include <string.h>

enum vm_value_type {
  NUMBER,
  OBJECT,
};

// different types of objects that
// are stored on the heap:
enum object_type {
  STRING,
  FUNCTION,
};
struct object {
  enum object_type type;
  void *data;
};

struct vm_value {
  enum vm_value_type type;
  union {
    double          number;
    struct object*  object;   // Objects will be stored on the heap. So store the pointer in vm_value 
  };
};

#define NUMBER(value) ((struct vm_value){NUMBER, .number = value})
#define AS_NUMBER(vm_value) ((double)vm_value.number)

#define AS_STRING(vm_value) ((char*)vm_value.object->data)

// returns a vm_value for string
struct vm_value ALLOC_STRING(const char *value) {

  // create string object
  void *str_obj = (void*)strdup(value);

  struct object *obj = (struct object*)malloc(sizeof(struct object));
  obj->type = STRING;
  obj->data = str_obj;

  struct vm_value result;
  result.type = OBJECT;
  result.object = obj;  // this is the same for some reason

  return result;
}



#endif 
