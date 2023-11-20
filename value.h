#ifndef VM_VALUES
#define VM_VALUES

#include <stdio.h>

enum vm_value_type {
  NUMBER,
};

struct vm_value {
  enum vm_value_type type;
  union {
    double number;
  };
};

#define NUMBER(value) ((struct vm_value){NUMBER, .number = value})

#endif 
