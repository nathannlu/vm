```
uint8_t bytecode[] = {
  OP_CONST,       0,                  // push NUMBER(42.0) onto the stack
  OP_SET_LOCAL,   0,                  // set locals[0] arr
  OP_CONST,       1,                  // push NUMBER(42.0) onto the stack
  OP_SET_LOCAL,   1,                  // set locals[0] arr
  OP_ADD, 
  OP_GET_LOCAL,   0,                  // set locals[0] arr
  OP_SCOPE_EXIT,  1,
//  OP_GET_LOCAL,   0,                  // set locals[0] arr
  OP_HALT
};

/**
 * testing bytecode: (set x (+ x 10))
 * result should be 20
 */
uint8_t bytecode[] = {
  OP_GET_GLOBAL,  3,                  // get function
  OP_CONST,       0,                  // push NUMBER(42.0) onto the stack
  OP_CONST,       1,                  // push NUMBER(64.0) onto the stack
  OP_CALL,        2,                  // call func w 2 parameters
  OP_SET_GLOBAL,  2,
  OP_GET_GLOBAL,  2,
  OP_HALT
};

/**
 * for loop testing bytecode
 */
uint8_t bytecode[] = {
  OP_CONST,         0,                 // store the index of the value in the constant pool
  OP_CONST,         1,
  OP_COMPARE,       1,
  OP_JMP_IF_FALSE,  0,0,
  OP_HALT
};
```
