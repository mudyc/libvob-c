
#include <stdlib.h>

#include "util/hashtable.h"


int HASH_SIZES[] = { 
	5, 13, 31,
	// http://planetmath.org/encyclopedia/GoodHashTablePrimes.html
	53,
	97,
	193,
	389,
	769,
	1543,
	3079,
	6151,
	12289,
	24593,
	49157,
	98317,
	196613,
	393241,
	786433,
	1572869,
	3145739,
	6291469,
	12582917,
	25165843,
	50331653,
	100663319,
	201326611,
	402653189,
	805306457,
	1610612741,
};


Hashtable *util_hashtbl_create(int(*hash_func)(void *key), 
			       bool (*equals_func)(void *a, void *b))
{
	int i;

	Hashtable *ret = malloc(sizeof(Hashtable));

	ret->hash_func = hash_func;
	ret->equals_func = equals_func;

	ret->size = 0;
	ret->allocation_idx = 0;
	ret->lookup_tbl = malloc(HASH_SIZES[ret->allocation_idx] 
				 * sizeof(struct linked_list));

	for (i=0; i<HASH_SIZES[ret->allocation_idx]; i++)
		ret->lookup_tbl[i] = NULL;

	return ret;
}

/** @return true if new added or false if key already exists.
 */
static bool put(Hashtable *tbl, struct linked_list **lookup, int tbl_size, void *key, void *value)
{
	int idx = abs(tbl->hash_func(key) % tbl_size);
	struct linked_list *ptr = lookup[idx];
	if (ptr == NULL) {
		ptr = lookup[idx] = malloc(sizeof(struct linked_list));
		ptr->next = NULL;
		ptr->key = key;
		ptr->value = value;

		return true;
	}
	// check for value update..
	while (ptr != NULL) {
		if (tbl->equals_func(key, ptr->key)) {
			ptr->value = value;
			return false;
		}
		ptr = ptr->next;
	}
	// add new chain item
	ptr = lookup[idx];
	while (ptr->next != NULL)
		ptr = ptr->next;

	ptr->next = malloc(sizeof(struct linked_list));
	ptr = ptr->next;
	ptr->next = NULL;
	ptr->key = key;
	ptr->value = value;
	return true;
}

void util_hashtbl_put(Hashtable *tbl, void *key, void *value)
{
	int i;

	// if time to resize...
	if (tbl->size > HASH_SIZES[tbl->allocation_idx] * 0.75) {
		struct linked_list **tmp = tbl->lookup_tbl;
		tbl->lookup_tbl = malloc(HASH_SIZES[tbl->allocation_idx + 1] 
				 * sizeof(struct linked_list));

		for (i=0; i<HASH_SIZES[tbl->allocation_idx]; i++) {
			struct linked_list *ptr = tmp[i];
			while (ptr != NULL) {
				put(tbl, tbl->lookup_tbl, 
				    HASH_SIZES[tbl->allocation_idx + 1], 
				    ptr->key, ptr->value);
				struct linked_list *old;
				ptr = ptr->next;
				free(old);
			}
		}

		tbl->allocation_idx++;
		free(tmp);
	}
	if (put(tbl, tbl->lookup_tbl, HASH_SIZES[tbl->allocation_idx], key, value))
		tbl->size++;
}

void *util_hashtbl_get(Hashtable *tbl, void *key)
{
	int idx = abs(tbl->hash_func(key) % HASH_SIZES[tbl->allocation_idx]);
	
	struct linked_list *ptr = tbl->lookup_tbl[idx];

	while (ptr != NULL) {
		if (tbl->equals_func(key, ptr->key))
			return ptr->value;
		ptr = ptr->next;
	}

	return NULL;
}


static bool has_next(HashtableIterator *i) 
{
	if (i->key == NULL)
		return false;
	
	int idx = abs(i->tbl->hash_func(i->key) % HASH_SIZES[i->tbl->allocation_idx]);
	struct linked_list *ptr = i->tbl->lookup_tbl[idx];
	while (ptr != NULL) {
		if (i->tbl->equals_func(i->key, ptr->key))
			if (ptr->next != NULL)
				return true;
		ptr = ptr->next;
	}
	for (; idx<HASH_SIZES[i->tbl->allocation_idx]; idx++) {
		ptr = i->tbl->lookup_tbl[idx];
		if (ptr != NULL)
			return true;
	}
	return false;
}
static void next(HashtableIterator *i)
{
	if (i->key == NULL)
		return;

	int idx = abs(i->tbl->hash_func(i->key) % HASH_SIZES[i->tbl->allocation_idx]);
	struct linked_list *ptr = i->tbl->lookup_tbl[idx];
	while (ptr != NULL) {
		if (i->tbl->equals_func(i->key, ptr->key))
			if (ptr->next != NULL) {
				i->key = ptr->next->key;
				i->value = ptr->next->value;
				return;
			}
		ptr = ptr->next;
	}
	for (; idx<HASH_SIZES[i->tbl->allocation_idx]; idx++) {
		ptr = i->tbl->lookup_tbl[idx];
		if (ptr != NULL) {
			i->key = ptr->next;
			i->value = ptr->value;
			return;
		}
	}
	
}
void util_hashtbl_iterator(Hashtable *tbl, HashtableIterator *i)
{
	int idx;

	i->tbl = tbl;

	i->has_next = &has_next;
	i->next = &next;

	for (idx=0; idx<HASH_SIZES[tbl->allocation_idx]; idx++) {
		struct linked_list *ptr = tbl->lookup_tbl[idx];
		if (ptr != NULL) {
			i->key = ptr->key;
			i->value = ptr->value;
		}
	}
}
