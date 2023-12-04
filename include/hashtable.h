#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define TABLE_SIZE 10

struct hashtable_entry {
  char* key;
  void* value;
  struct hashtable_entry* next;
};

struct hashtable {
  int size;
  struct hashtable_entry** entries;
};

struct hashtable* hashtable_create();
unsigned int hashtable_hash(const char *key);
void hashtable_set(struct hashtable* table, char* key, void* value);
void* hashtable_lookup(struct hashtable* table, const char *key);
void hashtable_dump(struct hashtable* table);

#endif
