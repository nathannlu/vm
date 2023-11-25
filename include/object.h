/**
 * VM values
 */
#ifndef OBJECT_H
#define OBJECT_H

enum object_type {
  STRING,
  CODE,
  FUNCTION,
};

// different types of objects that
// are stored on the heap:
struct object {
  enum object_type type;
	//struct list_node trace;
  void *data;
  /*
  union {
    //struct func_object* function
    struct code_object* code;
    // char* string
  };
  */
};


#endif
