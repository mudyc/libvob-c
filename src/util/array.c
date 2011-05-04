// (c): Matti J. Katila

#include <stdlib.h>
#include <stdio.h>

#include "util/array.h"



UtilArray *util_arr_create(Region *reg)
{
	UtilArray *ret = REGION(reg, "util.Array", UtilArray);
	//UtilArray *ret = malloc(sizeof(UtilArray));
	ret->data = NULL;
	ret->size = 0;
	ret->capacity = 0;
	return ret;
}


void util_arr_add(Region *reg, UtilArray *arr, void *data)
{
	int inc;
	//printf("util_arr_add %x %d %d \n", arr, arr->capacity, arr->size);
	if (arr->capacity <= arr->size) {
		//printf("util_arr_add inc\n");
		inc = arr->capacity*3/2;
		inc = inc > 4? inc: 4;
		//arr->data = malloc(inc * sizeof(void *));
		//arr->data = REGION(reg, "util.Array.data", /*inc*sizeof(*/void */*)*/);
		arr->data = util_regs_data_increase(reg, arr, arr->data, 
						    &arr->capacity,
						    inc * sizeof(void *));
		//for (i=0; i<arr->capacity; i++)
		//	arr->data[i] = tmp[i];
		//if (tmp != NULL)
		//	free(tmp);
		arr->capacity = inc;
	}
	//printf("util_arr_add ass\n");
	arr->data[arr->size++] = data;
	//printf("array size is: %d\n", arr->size);
}


void *util_arr_get(UtilArray *arr, int idx)
{
	if (idx < 0 || idx >= arr->size)
		return NULL;
	return arr->data[idx];
}

void util_arr_set(UtilArray *arr, int idx, void *data)
{
	if (idx < 0 || idx >= arr->size)
		arr->data[idx] = data;
	else 
		printf("FATAL: util array bounds check failed (0 <= %d < %d)!\n", idx, arr->size);
}

char *util_arr_get_str(UtilArray *arr, int idx)
{
	return (char *) util_arr_get(arr, idx);
}
