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
bool vm_value_cmp(struct vm_value value1, struct vm_value value2);

// returns a vm_value
struct vm_value ALLOC_OBJECT(void *value);

// returns a vm_value for string
struct vm_value ALLOC_STRING(const char *value);

struct vm_value ALLOC_NATIVE_FUNC(void* handler);
/**
 * Allocate space on the heap for globals array
 */
struct vm_value* ALLOC_CONSTANTS(struct vm_value* values);

#define NUMBER(value, ...) ((struct vm_value){NUMBER, ##__VA_ARGS__, .number = value})

#define AS_NUMBER(vm_value) ((double)vm_value.number)

#define BOOLEAN(value) ((struct vm_value){BOOLEAN, .boolean = value})
#define AS_BOOLEAN(vm_value) ((bool)vm_value.boolean)

#define AS_STRING(vm_value) ((char*)vm_value.object->data)

#endif 
