#ifndef CODE_OBJECT_H 
#define CODE_OBJECT_H 

#include "value.h"
#include "global.h"
#include "local.h"
#include "object.h"

struct code_object {
  char name[MAX_GLOBAL_NAME_SIZE];

  struct vm_value *constants;
  struct global *globals;
  struct local *locals;

  // bytecode;
  uint8_t *co;
};

// global funcs
void test_func() {
  printf("hello world");
}

uint8_t* ALLOC_BYTECODE(uint8_t* values, size_t size) {
  uint8_t* array = (uint8_t*)allocate_heap(size * sizeof(uint8_t));

  // Copy values to the dynamic array
  for (size_t i = 0; i < size; ++i) {
    array[i] = values[i];
  }

  return array;
}

struct code_object* ALLOC_CODE(const char* name, struct vm_value* constants, struct global* globals, struct local* locals, uint8_t* bytecode) {

  size_t co_obj_size = sizeof(struct code_object);
  struct code_object *co_obj = (struct code_object*)allocate_heap(co_obj_size);

  strncpy(co_obj->name, name, sizeof(co_obj->name) - 1);
  co_obj->name[sizeof(co_obj->name) - 1] = '\0'; 
  co_obj->constants = ALLOC_CONSTANTS(constants);
  co_obj->globals = ALLOC_GLOBALS(globals);
  co_obj->locals = ALLOC_LOCALS(locals);
  co_obj->co = ALLOC_BYTECODE(bytecode, 256);

  return co_obj;
}

struct code_object* alloc_func1() {
  uint8_t bytecode[] = {
    OP_CONST,   0,  // push NUMBER(42.0) onto the stack
    OP_CONST,   1,  // push NUMBER(64.0) onto the stack
    OP_ADD,
    OP_SCOPE_EXIT, 0,  // pop everything but the result
    OP_RETURN,
  };
  struct vm_value constants[10] = {
    NUMBER(42.0),
    NUMBER(64.0),
  };
  struct global globals[10] = {
    {"x", NUMBER(0.0)},
  };
  // index on stack
  struct local locals[10] = {
    {"y", 0},
  };

  struct code_object* co = ALLOC_CODE("func1", constants, globals, locals, bytecode);
  return co;
}

struct code_object* alloc_main() {
  struct vm_value func_1 = ALLOC_OBJECT((void*)alloc_func1());

  uint8_t bytecode[] = {
    OP_CONST,   1,  // push NUMBER(10.0) onto the stack
    OP_CONST,   0,  // push func onto the stack
    OP_CALL,    0,
    OP_ADD,
    OP_HALT
  };

  struct vm_value constants[10] = {
    func_1,
    NUMBER(10.0),
  };
  struct global globals[10] = {
    {"x", NUMBER(10.0)},
    {"y", NUMBER(20.0)},
  //  {"square", ALLOC_FUNC((void*)&test_func)},
    //{"sum", ALLOC_FUNC((void*)&sum_func)},
  };
  // index on stack
  struct local locals[10] = {
    {"a", 0},
    {"b", 1},
  };

  struct code_object* co = ALLOC_CODE("main", constants, globals, locals, bytecode);
  return co;
}





#endif
