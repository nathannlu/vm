- scoping for local variables
- in main.c, struct constants pool is statically defined. it needs to generate constants from bytecode or from compiler

- move scope up instead of setting it to NULL in ```OP_SCOPE_EXIT```
