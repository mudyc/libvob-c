// (c): Matti J. Katila


#ifndef UTIL_ARRAY_H
#define UTIL_ARRAY_H


struct util_array {
	int size;
	int capacity;
	void **data;
};
typedef struct util_array UtilArray;


UtilArray *util_arr_create();


void util_arr_add(UtilArray *arr, void *data);

/**
 * for (int i=0; i<arr->size; i++) {
 *     char *str = util_arr_get(i);
 * }
 */
void *util_arr_get(UtilArray *arr, int idx);

char *util_arr_get_str(UtilArray *arr, int idx);


#endif
