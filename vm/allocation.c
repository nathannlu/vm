#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "allocation.h"



void initialize_allocation_list(struct _allocation_list* list, size_t capacity) {
  list->allocations = (struct allocation*)malloc(sizeof(struct allocation) * capacity);
  list->capacity = capacity;
  list->count = 0;
}

void allocation_list_push(struct _allocation_list* list, void* addr, size_t size) {
  if (list->count < list->capacity) {
    list->allocations[list->count].address = addr;
    list->allocations[list->count].size = size;
    list->count++;
  } else {
    printf("No more space in the allocations array\n");
  }
}

void allocation_list_free(struct _allocation_list* list) {
  free(list->allocations);
}

/** 
 * Frees all objects
 */
void allocation_list_objects_free(struct _allocation_list* list) {
  for (size_t i = 0; i < list->count; i++) {
    free(list->allocations[i].address);
    bytes_allocated -= list->allocations[i].size;
  }

  free(list->allocations);
}



// Track allocation manually
void track_allocation(void*address, size_t size) {
  allocation_list_push(&allocation_list, address, size);

  /*
  struct allocation* new_allocation = (struct allocation*)malloc(sizeof(struct allocation));
  // Fill in the details
  new_allocation->address = address;
  new_allocation->size = size;
  new_allocation->next = allocation_list;

  // Update the allocation list
  allocation_list = new_allocation;
  */

  // Update total bytes allocated
  bytes_allocated += size;
}

// General malloc function
void *allocate_heap(size_t size) {
  void *ptr = malloc(size);
  track_allocation(ptr, size);

  return ptr;
}

// Function to free all tracked allocations
/*
void free_allocations() {
  while (allocation_list != NULL) {
    // @question
    // does this free to allocation struct
    // or the data the struct refers to?
    struct allocation* current = allocation_list;
    void *obj = current->address;  
    free(obj);

    // Remove from total bytes allocated
    //bytes_allocated -= current->size;

    allocation_list = allocation_list->next;
    free(current);
  }
}
*/
