#include "codeobject.h"

uint8_t* ALLOC_BYTECODE(uint8_t* values, size_t size) {
  uint8_t* array = (uint8_t*)allocate_heap(size * sizeof(uint8_t));

  // Copy values to the dynamic array
  for (size_t i = 0; i < size; ++i) {
    array[i] = values[i];
  }

  return array;
}

struct code_object* new_code_object(const char* name, struct vm_value* constants, struct globals* globals, struct locals* locals, uint8_t* bytecode) {

  size_t co_obj_size = sizeof(struct code_object);
  struct code_object *co_obj = (struct code_object*)allocate_heap(co_obj_size);

  strncpy(co_obj->name, name, sizeof(co_obj->name) - 1);
  co_obj->name[sizeof(co_obj->name) - 1] = '\0'; 
  co_obj->constants = constants;
  co_obj->globals = globals;
  co_obj->locals = locals;
  co_obj->bytecode = ALLOC_BYTECODE(bytecode, 256);

  return co_obj;
}

