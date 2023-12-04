#include "codeobject.h"
#include "global.h"
#include "local.h"

#define MAX_BYTECODE_LENGTH 254

/*
uint8_t* ALLOC_BYTECODE(uint8_t* values, size_t size) {
  uint8_t* array = (uint8_t*)allocate_heap(size * sizeof(uint8_t));

  // Copy values to the dynamic array
  for (size_t i = 0; i < size; ++i) {
    array[i] = values[i];
  }

  return array;
}
*/

struct code_object* new_code_object(const char* name) {

  size_t co_obj_size = sizeof(struct code_object);
  struct code_object *co_obj = (struct code_object*)allocate_heap(co_obj_size);





  uint8_t* bytecode = (uint8_t*)allocate_heap(sizeof(uint8_t) * MAX_BYTECODE_LENGTH);
  struct globals*   globals   = (struct globals*)allocate_heap(sizeof(struct globals));
  struct locals*    locals    = (struct locals*)allocate_heap(sizeof(struct locals));
  struct constants* constants = (struct constants*)allocate_heap(sizeof(struct constants));

  initialize_globals(globals);
  initialize_locals(locals);
  initialize_constants(constants);

  //size_t constants_array_size = 100;
  //struct vm_value* constants = (struct vm_value*)allocate_heap(constants_array_size * sizeof(struct vm_value));


  strncpy(co_obj->name, name, sizeof(co_obj->name) - 1);
  co_obj->name[sizeof(co_obj->name) - 1] = '\0'; 
  co_obj->constants = constants;
  co_obj->globals = globals;
  co_obj->locals = locals;
  co_obj->bytecode = bytecode;

  return co_obj;
}

