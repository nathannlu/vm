/**
 * VM values
 */
#ifndef OBJECT_H
#define OBJECT_H

#include "codeobject.h"

enum object_type {
  STRING,
  CODE,
  FUNCTION,
};

// different types of objects that
// are stored on the heap:
struct object {
  enum object_type type;

  void *data; // @to be removed

  union {
    //struct func_object* function
    struct code_object* code;
    char*               string;
  };
};

struct object* new_object_code_object(struct code_object *code);
struct object* new_object_string_object(const char *value);

#define object_to_vm_value(obj) ((struct vm_value){OBJECT, .object = obj})

#define vm_value_to_string(vm_value) ((char*)vm_value.object->string)
#define vm_value_to_code(vm_value) ((struct code_object*)vm_value.object->code)
#define vm_value_to_native_function(vm_value) ((void*)vm_value.object->data)


#endif
