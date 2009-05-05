// (c): Matti J. Katila

#ifndef UTIL_REGIONS_H
#define UTIL_REGIONS_H

#include <stdlib.h>
#include <glib.h>

//#include "util/strings.h"
#include "util/hashtable.h"


#define REGION_ID(ID, NAME) \
     	static RegionId ID = { .id = NULL }; \
	if (ID.id == NULL) \
		util_regs_id_init(&ID, NAME);


struct util_regs_id {
	char *id;
	int hash;
};
typedef struct util_regs_id RegionId;

void util_regs_id_init(RegionId *r, char *id);



struct util_fast_array {
	size_t blob_size;
	int blobs_in_full_chunk;
	int buff_size;

	// current free index
	int index;
	// allocation size
	int size;

	/*- It's more efficient to have chunks of 4096 bytes of memory
	 * than a lot of more in one slice. So we have a chunk which
	 * points to other chunks.
	 *
	 * +-----------------------------------+
	 * | chunks                            |
	 * +-----------------------------------+
	 *   |        |    ...  |
	 *   v        v         v
	 *  +-+      +-+       +-+
	 *  |c|      |c|       |c|
	 *  |h|      |h|       |h|
	 *  |u|      |u|       |u|
	 *  |n|      |n|       |n|
	 *  |k|      |k|       |k|
	 *  +-+      +-+       +-+
	 */
	char **chunks;
};
typedef struct util_fast_array FastArray;

FastArray *util_fastarr_create(size_t size);
void util_fastarr_add(FastArray *arr, void *data);
void *util_fastarr_get(FastArray *arr, int index);
void util_fastarr_clear(FastArray *arr);



struct util_region {
	GHashTable *regid2fastarr;
	char *name; // if any
};
typedef struct util_region Region;


Region *util_regs_create(char *name);


/** Instantiate memory object in region.
 * @return A pointer to uninitialized memory structure.
 */
void *util_regs_instantiate(Region *reg, RegionId *id, size_t size);

/** Marks all objects available for reuse within the region.
 */
void util_regs_clear(Region *reg);


#endif
