#include <stddef.h>
#include <stdlib.h>
#include "allocation.h"

struct allocation* allocation_list = NULL;
size_t bytes_allocated = 0;

// Track allocation manually
void track_allocation(void*address, size_t size) {
  struct allocation* new_allocation = (struct allocation*)malloc(sizeof(struct allocation));

  // Fill in the details
  new_allocation->address = address;
  new_allocation->size = size;
  new_allocation->next = allocation_list;

  // Update the allocation list
  allocation_list = new_allocation;

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
