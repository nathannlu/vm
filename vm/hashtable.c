#include "hashtable.h"

struct hashtable* hashtable_create() {
  // Allocate base table struct
  struct hashtable* ht = (struct hashtable*)malloc(sizeof(struct hashtable) * 1); 

  // Allocate table entries
  ht->entries = malloc(sizeof(struct hashtable_entry) * TABLE_SIZE);

  for (int i = 0; i < TABLE_SIZE; i++) {
    ht->entries[i] = NULL;
  }

  return ht;
}


// Hashes a key and returns its index
unsigned int hashtable_hash(const char *key) {
  unsigned long int value = 0;
  unsigned int key_len = strlen(key);

  for (int i = 0; i < key_len; i++) {
    value = value * 37 + key[i];
  }

  value = value % TABLE_SIZE;

  return value;
}

static struct hashtable_entry* new_entry(char* key, void* value) {
  struct hashtable_entry* new_kv = (struct hashtable_entry*)malloc(sizeof(struct hashtable_entry));
  new_kv->key = key;
  new_kv->value = value;

  return new_kv;
}


void hashtable_set(struct hashtable* table, char* key, void* value) {
  // Hash the key
  unsigned int slot = hashtable_hash(key);

  struct hashtable_entry* entry = table->entries[slot];
  if(entry == NULL) {
    table->entries[slot] = new_entry(key, value);
    return;
  }

  
  struct hashtable_entry* prev;
  while (entry != NULL) {
    // Check key
    if (strcmp(entry->key, key) == 0) {
      // Match found, replace value
      free(entry->value);
      entry->value = value;
      return;
    } 

    // Walk to next entry
    prev = entry;
    entry = prev->next;
  }

  // End of chain reached without a match, add new
  prev->next = new_entry(key, value);
}

void* hashtable_lookup(struct hashtable* table, const char *key) {
  unsigned int slot = hashtable_hash(key);

  struct hashtable_entry* entry = table->entries[slot];

  if (entry == NULL) {
    return NULL;
  }

  while (entry != NULL) {
    if (strcmp(entry->key, key) == 0) {
      return entry->value; 
    }

    // Walk to next entry
    entry = entry->next;
  }

  // No match found
  return NULL;
}

void hashtable_dump(struct hashtable* table) {
  for(int i = 0; i < TABLE_SIZE; i++) {
    struct hashtable_entry* entry = table->entries[i];

    if (entry == NULL) {
      continue;
    }

    printf("slot[%4d]: ",i);

    for (;;) {
      printf("%s=%p ", entry->key, entry->value);

      if(entry->next == NULL) {
        break;
      }

      entry = entry->next;
    }

    printf("\n");
  }
}

