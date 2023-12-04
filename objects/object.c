#include "object.h"
#include "allocation.h"

// returns a vm_value
struct object* new_object_code_object(struct code_object *code) {
  // create base object
  size_t obj_size = sizeof(struct object);
  struct object *obj = (struct object*)allocate_heap(obj_size);
  obj->type = CODE;
  obj->code = code;

  return obj;
}

// returns a vm_value for string
struct object* new_object_string_object(const char *value) {

  // create string object
  size_t str_size = strlen(value) + 1;
  void *str_obj = (void*)strdup(value);
  track_allocation(str_obj, str_size);

  // create base object
  size_t obj_size = sizeof(struct object);
  struct object *obj = (struct object*)allocate_heap(obj_size);
  obj->type = STRING;
  obj->string = str_obj;

  return obj;
}
