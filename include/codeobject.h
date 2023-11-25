#ifndef CODE_OBJECT_H 
#define CODE_OBJECT_H 

#include <stdint.h>
#include "value.h"
#include "global.h"
#include "local.h"
#include "object.h"
#include "op_code.h"

struct code_object {
  char name[MAX_GLOBAL_NAME_SIZE];

  struct vm_value *constants;
  struct global *globals;
  struct local *locals;

  // bytecode;
  uint8_t *co;
};

uint8_t* ALLOC_BYTECODE(uint8_t* values, size_t size);

struct code_object* ALLOC_CODE(const char* name, struct vm_value* constants, struct global* globals, struct local* locals, uint8_t* bytecode); 
//struct code_object* alloc_func1();
struct code_object* alloc_main();

#endif
