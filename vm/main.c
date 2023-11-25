#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "value.h"
#include "codeobject.h"
#include "allocation.h"
#include "stack.h"
#include "callstack.h"
#include "op_code.h"

#include "eval.h"

struct _allocation_list allocation_list;
size_t bytes_allocated = 0;

void exec() {
  initialize_allocation_list(&allocation_list, 256);
  return run();
}

int main() {
  exec();

  printf("Bytes allocated: %zu\n", bytes_allocated);
  allocation_list_objects_free(&allocation_list);
  printf("Bytes allocated: %zu\n", bytes_allocated);

  return 0;
}
