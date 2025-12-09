#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct {
	uint64_t a;
	uint64_t b;
} Key;

typedef struct {
	Key key;
	uint64_t value;
	bool used;
} Entry;

typedef struct {
	Entry *entries;
	size_t capacity;
	size_t size;
} HashTable;



uint64_t hash(uint64_t x);
uint64_t hashKey(Key key);
void initializeHashTable(HashTable *table, size_t capacity);
void freeHashTable(HashTable *table);
void insertKey(HashTable *table, Key key, uint64_t value);
bool lookUpKey(HashTable *table, Key key, uint64_t *out);
bool containsKey(HashTable *table, Key key);
void clearHashTable(HashTable *table);
