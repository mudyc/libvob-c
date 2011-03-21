
#include <string.h>

#include "util/regions.h"
#include "util/dbg.h"
#include "util/strings.h"



// ======================================
// Fast array implementation
// ======================================

static const int ARRAY_MAX = 4096;
static const int ARRAY_SIZES[] = {0, 4, 16, 64, 256, 1024, 4096 };
static const int ARRAY_SIZES_N = sizeof(ARRAY_SIZES) / sizeof (ARRAY_SIZES[0]);

FastArray *util_fastarr_create(size_t size)
{
	FastArray *ret = malloc(sizeof(FastArray));
	ret->blob_size = size;
	ret->blobs_in_full_chunk = ARRAY_MAX / size;
	ret->buff_size = 0;
	ret->index = 0;
	ret->size = 0;
	ret->chunks = NULL;
	return ret;
}

static void *array_next(FastArray *arr)
{
	int i,j;
	int column = arr->index / arr->blobs_in_full_chunk;
	int row = arr->index % arr->blobs_in_full_chunk;

	//printf("col %d row %d\n", column, row);

	if (arr->index >= arr->size) {


		if (row == 0) { // we need to increase chunks array
			char **tmp = arr->chunks;
			arr->chunks = malloc((1 + column)*sizeof(char*));
			for (i=0; i<column; i++)
				arr->chunks[i] = tmp[i];
			arr->chunks[column] = NULL;
			if (tmp != NULL)
				free(tmp);
		}

		// ok, we need to resize the slice..
		int at_least = (1+row) * arr->blob_size;
		for (i=0; i<ARRAY_SIZES_N; i++) {
			if (ARRAY_SIZES[i] >= at_least) {
				char *tmp = arr->chunks[column];
				arr->chunks[column] = malloc(ARRAY_SIZES[i]*sizeof(char));
				char *new = arr->chunks[column];
				if (tmp != NULL) {
					for (j=0; j<ARRAY_SIZES[i-1]; j++)
						new[j] = tmp[j];
					free(tmp);
				}
				// update size info
				arr->size = (column * arr->blobs_in_full_chunk) +
					(ARRAY_SIZES[i] / arr->blob_size); 
				break;
			}
		}
	}
	arr->index++;
	return &arr->chunks[column][row * arr->blob_size];
}

void util_fastarr_add(FastArray *arr, void *data)
{
	void **ptr = array_next(arr);
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
	Region *ret =  malloc(sizeof(Region));
	ret->regid2fastarr = g_hash_table_new(&g_direct_hash,
					      &g_direct_equal);
	ret->name = name;
	return ret;
}


void *util_regs_instantiate(Region *reg, void *id, size_t size)
{
	//printf("util_regs_instantiate: %x %x %x\n", reg, id, size);
	FastArray *arr = g_hash_table_lookup(reg->regid2fastarr, id);
	if (arr == NULL)
		g_hash_table_insert(reg->regid2fastarr, id, 
				 arr = util_fastarr_create(size));

	//printf("instantiate: %d\n", arr->index);
	return array_next(arr);
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
