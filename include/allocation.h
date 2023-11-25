/**
 * Tracks allocating memory on the heap
 */

#ifndef ALLOCATION_H
#define ALLOCATION_H

#include <stddef.h>

// Total number of allocated bytes
struct allocation {
  void *address;            // mem address of the allocated obj
  size_t size;
};

struct _allocation_list {
  struct allocation* allocations;
  size_t capacity;
  size_t count;
};

extern struct _allocation_list allocation_list;
extern size_t bytes_allocated;


void initialize_allocation_list(struct _allocation_list* list, size_t capacity);
void allocation_list_push(struct _allocation_list* list, void* addr, size_t size);
void allocation_list_free(struct _allocation_list* list);
void allocation_list_objects_free(struct _allocation_list* list);

void track_allocation(void*address, size_t size);
void *allocate_heap(size_t size);
//void free_allocations();

#endif 
