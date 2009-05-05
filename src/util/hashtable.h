#ifndef UTIL_HASHTABLE_H
#define UTIL_HASHTABLE_H

#include <stdbool.h>

struct linked_list {
	void *key;
	void *value;
	struct linked_list *next;
};


struct util_hashtable {
	int size;
	int allocation_idx;
	struct linked_list **lookup_tbl;

	int (*hash_func)(void *key);
	bool (*equals_func)(void *a, void *b);
};
typedef struct util_hashtable Hashtable;

struct util_hashtable_iterator;
typedef struct util_hashtable_iterator HashtableIterator;
struct util_hashtable_iterator {
	void *key;
	void *value;
	bool (*has_next)(HashtableIterator *i);
	void (*next)(HashtableIterator *i);

	Hashtable *tbl;
};
//typedef struct util_hashtable_iterator HashtableIterator;

Hashtable *util_hashtbl_create(int(*hash_func)(void *key), bool (*equals_func)(void *a, void *b));

void util_hashtbl_put(Hashtable *tbl, void *key, void *value);
void *util_hashtbl_get(Hashtable *tbl, void *key);

void util_hashtbl_iterator(Hashtable *tbl, HashtableIterator *i);




#endif
