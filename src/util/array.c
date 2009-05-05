// (c): Matti J. Katila

#include <stdlib.h>

#include "util/array.h"



UtilArray *util_arr_create()
{
	UtilArray *ret = malloc(sizeof(UtilArray));
	ret->data = NULL;
	ret->size = 0;
	ret->capacity = 0;
	return ret;
}


void util_arr_add(UtilArray *arr, void *data)
{
	int i, inc;
	if (arr->capacity <= arr->size) {
		void **tmp = arr->data;
		inc = arr->capacity*3/2;
		inc = inc > 4? inc: 4;
		arr->data = malloc(inc * sizeof(void *));
		for (i=0; i<arr->capacity; i++)
			arr->data[i] = tmp[i];
		if (tmp != NULL)
			free(tmp);
		arr->capacity = inc;
	}
	arr->data[arr->size++] = data;
}


void *util_arr_get(UtilArray *arr, int idx)
{
	if (idx < 0 || idx >= arr->size)
		return NULL;
	return arr->data[idx];
}

char *util_arr_get_str(UtilArray *arr, int idx)
{
	return (char *) util_arr_get(arr, idx);
}
