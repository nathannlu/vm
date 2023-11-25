#ifndef CODE_OBJECT_H 
#define CODE_OBJECT_H 

#include <stdint.h>
#include "value.h"
#include "local.h"
#include "object.h"
#include "op_code.h"
#include "global.h"

#define MAX_CODE_OBJECT_NAME_SIZE 10

struct code_object {
  char name[MAX_CODE_OBJECT_NAME_SIZE];

  struct vm_value *constants;
  struct global *globals;
  struct local *locals;

  // bytecode;
  uint8_t *co;
  uint8_t *bytecode;
};

uint8_t* ALLOC_BYTECODE(uint8_t* values, size_t size);

//struct code_object* new_code_object(const char* name, struct vm_value* constants, struct global* globals, struct local* locals, uint8_t* bytecode); 
//struct code_object* new_code_object(const char* name, struct vm_value* constants, struct local* locals, uint8_t* bytecode); 
struct code_object* alloc_main();

#endif
