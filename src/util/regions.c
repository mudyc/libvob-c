
#include <string.h>
#include <mcheck.h>

#include "util/regions.h"
#include "util/dbg.h"
#include "util/strings.h"



// ======================================
// Fast array implementation
// ======================================

static const int ARRAY_MAX = 4096;

/* Stretching slices won't work as objects may have pointers!
 */
static const int ARRAY_SIZES[] = {0, 4, 16, 64, 256, 1024, 4096 };
static const int ARRAY_SIZES_N = sizeof(ARRAY_SIZES) / sizeof (ARRAY_SIZES[0]);

static GHashTable *dbg_objects = NULL;

FastArray *util_fastarr_create(size_t size)
{
	FastArray *ret = malloc(sizeof(FastArray));
	ret->blob_size = size;
	ret->blobs_in_full_chunk = ARRAY_MAX / size;
	ret->buff_size = 0;
	ret->index = 0; // current index in units
	ret->capacity = 0;  // capacity in units
	ret->chunks = NULL;
	return ret;
}

static void *array_next(FastArray *arr)
{
	int i,j;
	int column = arr->index / arr->blobs_in_full_chunk;
	int row = arr->index % arr->blobs_in_full_chunk;

	//printf("col %d row %d\n", column, row);
/*	for (i=0; i<=column; i++)
		if (arr->chunks != NULL)
			printf("%p, ", arr->chunks[0]);
	printf("\n");
*/


	if (arr->index >= arr->capacity) {
		if (row == 0) { // we need to increase chunks array
			//printf("inc chunks..\n");
			void **tmp = arr->chunks;
			arr->chunks = malloc((1 + column)*sizeof(void*));
			memcpy(arr->chunks, tmp, column*sizeof(void*));
			//for (i=0; i<column; i++)
			//	arr->chunks[i] = tmp[i];
			arr->chunks[column] = NULL;
			if (tmp != NULL)
				free(tmp);
		}
		arr->chunks[column] = malloc(ARRAY_MAX);
		arr->capacity = (column * arr->blobs_in_full_chunk) +
			(ARRAY_MAX / arr->blob_size);
		/*
		// ok, we need to resize the slice..
		int at_least = (1+row) * arr->blob_size;
		//printf("at least: %d\n", at_least);
		for (i=0; i<ARRAY_SIZES_N; i++) {
			if (ARRAY_SIZES[i] >= at_least) {
				char *tmp = arr->chunks[column];
				void *data = NULL;
				data = malloc(ARRAY_SIZES[i]*sizeof(char));
				arr->chunks[column] = data;
				char *new = arr->chunks[column];
				printf("malloced: %x %d\n", new, ARRAY_SIZES[i]);
				if (tmp != NULL) {
					memcpy(data, new, ARRAY_SIZES[i-1]);
					free(tmp);
				}
				// update size info
				arr->capacity = (column * arr->blobs_in_full_chunk) +
					(ARRAY_SIZES[i] / arr->blob_size); 
				break;
			}
		}
		*/
	}
	arr->index++;
	//printf("ret.. %d ", row * arr->blob_size);
	return &arr->chunks[column][row * arr->blob_size];
}

void util_fastarr_add(FastArray *arr, void *data)
{
	void **ptr = (void **)array_next(arr);
	ptr[0] = data;
}

void *util_fastarr_get(FastArray *arr, int idx)
{
	if (idx < 0 || arr->index <= idx) {
		fprintf(stderr, "Array index check failed (given: %d, size: %d)\n", idx, arr->index);
		return NULL;
	}
	int column = idx / arr->blobs_in_full_chunk;
	int row = idx % arr->blobs_in_full_chunk;
	void **ret = (void **) &arr->chunks[column][row * arr->blob_size];
	//printf("fat arr get %p\n", ret);
	return *ret;
}

void util_fastarr_clear(FastArray *arr)
{
	arr->index = 0;
}


// ======================================
// Region implementation
// ======================================

/*
void util_regs_id_init(Region *r, int *id, char *idstr)
{
        // http://www.informatics.susx.ac.uk/courses/dats/notes/html/node114.html
        // http://computinglife.wordpress.com/2008/11/20/why-do-hash-functions-use-prime-numbers/
	int i;
	int len = strlen(id);
	r->hash = 0;
	r->id = id;
	for (i=0; i<len; i++)
		r->hash = 31*r->hash + id[i];
}
*/

Region *util_regs_create(char *name)
{
	Region *ret = malloc(sizeof(Region));
	ret->regid2fastarr = g_hash_table_new(&g_direct_hash,
					      &g_direct_equal);
	ret->owner2data = g_hash_table_new(&g_direct_hash,
					   &g_direct_equal);
	ret->name = name;
	return ret;
}


void *util_regs_instantiate(Region *reg, void *id, size_t size)
{
	//printf("util_regs_instantiate: %s %x %x %d\n", id, reg, id, size);

	FastArray *arr = g_hash_table_lookup(reg->regid2fastarr, id);
	if (arr == NULL)
		g_hash_table_insert(reg->regid2fastarr, id, 
				 arr = util_fastarr_create(size));

	//printf("instantiate: %x %s %d %d\n", id, id, arr->index, size);
	void *ret = array_next(arr);
	//printf("instantiated: %s %p %p\n", id, ret, arr);

	if (dbg_objects)
		g_hash_table_insert(dbg_objects, ret, id);

	return ret;
}

void *util_regs_data_increase(Region *reg, void *owner, void *curr_mem,
			      size_t capacity, size_t size)
{
	//printf("util_regs_inc\n");
	void **data = g_hash_table_lookup(reg->owner2data, owner);
	if (data != curr_mem) {
		printf("FATAL: Region data increased with wrong data.");
		exit(1);
	}
	void **tmp = data;
	data = malloc(size);
	if (tmp != NULL) {
		memcpy(data, tmp, capacity);
		free(tmp);
	}
	g_hash_table_insert(reg->owner2data, owner, data);
	return data;
}


void util_regs_clear(Region *reg)
{
	//printf("regs clear %s %d\n ", reg->name==NULL?"":reg->name, 
	//       g_hash_table_size(reg->regid2fastarr));

	GHashTableIter iter;
	gpointer key, value;

	g_hash_table_iter_init(&iter, reg->regid2fastarr);
	for (; g_hash_table_iter_next(&iter, &key, &value); ) {
		//printf("cleared.. %s\n", ((RegionId *)key)->id);
		FastArray *arr = (FastArray*) value;
		util_fastarr_clear(arr);
	}
}

char *util_regs_dbg(void *object) {
	if (dbg_objects == NULL)
		dbg_objects = g_hash_table_new(&g_direct_hash, &g_direct_equal);
	return g_hash_table_lookup(dbg_objects, object);
}
