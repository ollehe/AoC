#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "hash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
static const size_t INITIAL_CAPACITY = 8192;

static inline bool keyEquality(Key a, Key b)
{
	return a.a == b.a && a.b == b.b;
}

static void resizeHashTable(HashTable *table, size_t newCapacity)
{
	printf("Resizing Hash Table. Current Capacity: %zu\n", table->capacity);
	HashTable newTable;
	initializeHashTable(&newTable, newCapacity);
	for (size_t i = 0; i < table->capacity; i++) {
		Entry e = table->entries[i];
		if (e.used) {
			insertKey(&newTable, e.key, e.value);
		}
	}
	free(table->entries);
	*table = newTable;
	printf("Hash Table Resized. New Capacity: %zu\n", newTable.capacity);
}

// Source - https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
// Retrieved 2025-12-08, License - CC BY-SA 4.0
uint64_t hash(uint64_t x)
{
	x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
	x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
	x = x ^ (x >> 31);
	return x;
}

// Use boost hash_combine
uint64_t hashKey(Key key)
{
	uint64_t h = hash(key.a);
	h ^= hash(key.b) + UINT64_C(0x9e3779b97f4a7c15) + (h << 6) + (h >> 2);

	return h;
}

void initializeHashTable(HashTable *table, size_t capacity)
{
	if (capacity == 0) {
		capacity = INITIAL_CAPACITY;
	}
	if ((capacity & (capacity - 1)) != 0) {
		fprintf(stderr, "Capacity must be a power of 2\n");
		exit(EXIT_FAILURE);
	}
	table->capacity = capacity;
	table->size = 0;
	table->entries = calloc(capacity, sizeof(Entry));
	if (!table->entries) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
}

void freeHashTable(HashTable *table)
{
	free(table->entries);
	table->entries = NULL;
	table->capacity = 0;
	table->size = 0;
}

bool lookUpKey(HashTable *table, Key key, uint64_t *out)
{
	uint64_t h = hashKey(key);
	size_t index = h & (table->capacity - 1);
	while (table->entries[index].used) {
		if (keyEquality(key, table->entries[index].key)) {
			*out = table->entries[index].value;
			return true;
		}
		index = (index + 1) & (table->capacity - 1);
	}
	return false;
}

void insertKey(HashTable *table, Key key, uint64_t value)
{
	if ((table->size * 4) >= (table->capacity * 3)) {
		resizeHashTable(table, table->capacity * 2);
	}
	uint64_t h = hashKey(key);
	size_t index = h & (table->capacity - 1);
	while (true) {
		Entry *entry = &table->entries[index];

		if (!entry->used) {
			entry->used = true;
			entry->key = key;
			entry->value = value;
			table->size++;
			return;
		}
		if (keyEquality(key, entry->key)) {
			entry->value = value;
			return;
		}
		index = (index + 1) & (table->capacity - 1);
	}
}

bool containsKey(HashTable *table, Key key)
{
	uint64_t h = hashKey(key);
	size_t index = h & (table->capacity - 1);
	while (table->entries[index].used) {
		if (keyEquality(key, table->entries[index].key)) {
			return true;
		}
		index = (index + 1) & (table->capacity - 1);
	}
	return false;
}

void clearHashTable(HashTable *table)
{
	memset(table->entries, 0, table->capacity * sizeof(Entry));
	table->size = 0;
}
