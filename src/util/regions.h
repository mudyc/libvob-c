// (c): Matti J. Katila

#ifndef UTIL_REGIONS_H
#define UTIL_REGIONS_H

#include <stdlib.h>
#include <glib.h>

//#include "util/strings.h"
//#include "util/hashtable.h"


#define REGION_ID(REG, ID, NAME) \
        util_regs_id_init(REG, &ID, NAME);



#define REGION(REG, ID, TYPE) \
        (TYPE *) util_regs_instantiate(REG, ID, sizeof(TYPE));


struct util_fast_array {
	size_t blob_size;
	int blobs_in_full_chunk;
	int buff_size;

	// current free index
	int index;
	// allocation size
	int capacity;

	/*- It's more efficient to have chunks of 4096 bytes of memory
	 * than a lot of more in one slice. So we have a chunk which
	 * points to other chunks. Of course this assumption may be
	 * wrong as well and probably it is.
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
	void **chunks;
};
typedef struct util_fast_array FastArray;

FastArray *util_fastarr_create(size_t size);
void util_fastarr_add(FastArray *arr, void *data);
void *util_fastarr_get(FastArray *arr, int index);
void util_fastarr_clear(FastArray *arr);



struct util_region {
	GHashTable *regid2fastarr;
	GHashTable *owner2data;
	char *name; // if any
};
typedef struct util_region Region;


Region *util_regs_create(char *name);


/** Instantiate memory object in region.
 * @param size is the "class" size.
 * @return A pointer to uninitialized memory structure.
 */
void *util_regs_instantiate(Region *reg, void *id, size_t size);

/** Intantiate method for collections.
 */
//void *util_regs_data_instantiate(Region *reg, void *owner, size_t size);
void *util_regs_data_increase(Region *reg, void *owner, void *curr_mem,
			      size_t capacity, size_t size);

/** Marks all objects available for reuse within the region.
 */
void util_regs_clear(Region *reg);


#endif
