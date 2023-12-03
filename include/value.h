/**
 * VM values
 */
#ifndef VM_VALUES
#define VM_VALUES

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "object.h"
#include "allocation.h"

#define MAX_CONSTANTS_ARRAY_SIZE 50

enum vm_value_type {
  NUMBER,
  BOOLEAN,
  OBJECT,
};

struct vm_value {
  enum vm_value_type  type;
  //int                 scope;
  union {
    double          number;
    struct object*  object;   // Objects will be stored on the heap. So store the pointer in vm_value 
    bool            boolean;
  };
};

// Struct for hosting a list of global variables
struct constants {
  struct vm_value constants[MAX_CONSTANTS_ARRAY_SIZE];
  int             count;
};

struct vm_value ALLOC_NATIVE_FUNC(void* handler);
//struct vm_value* ALLOC_CONSTANTS(struct vm_value* values);

void initialize_constants(struct constants* constants);
int add_constant(struct constants* constants_arr, struct vm_value* value);

#define NUMBER(value) ((struct vm_value){NUMBER, .number = value})

#define AS_NUMBER(vm_value) ((double)vm_value.number)

#define BOOLEAN(value) ((struct vm_value){BOOLEAN, .boolean = value})
#define AS_BOOLEAN(vm_value) ((bool)vm_value.boolean)

//#define AS_STRING(vm_value) ((char*)vm_value.object->data)

#endif 
