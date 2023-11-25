#ifndef ALLOCATION_H
#define ALLOCATION_H

#include <stddef.h>

// Total number of allocated bytes
struct allocation {
  void *address;            // mem address of the allocated obj
  size_t size;
  struct allocation* next;  // next 'allocation' object
};

/*
extern struct allocation* allocation_list;
extern size_t bytes_allocated;
*/

void track_allocation(void*address, size_t size);
void *allocate_heap(size_t size);
void free_allocations();

#endif 
