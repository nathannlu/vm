#ifndef ALLOCATION_H
#define ALLOCATION_H

// Total number of allocated bytes
struct allocation {
  void *address;            // mem address of the allocated obj
  size_t size;
  struct allocation* next;  // next 'allocation' object
};
struct allocation* allocation_list = NULL;
size_t bytes_allocated = 0;

void track_allocation(void*address, size_t size);
void *allocate_heap(size_t size);
void free_allocations();

#endif 
