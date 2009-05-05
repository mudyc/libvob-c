// (c): Matti J. Katila

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/types.h>
#include <regex.h>

#include "util/strings.h"


// http://www.informatics.susx.ac.uk/courses/dats/notes/html/node114.html
static int str_hash(char *buff)
{
	int i;
	int len = strlen(buff);
	int ret = 0;
	for (i=0; i<len; i++)
		ret = 31*ret + buff[i];
	return ret;
}

UtilString *util_str_create(char *buff)
{
	UtilString *ret = malloc(sizeof(UtilString));
	ret->hash = str_hash(buff);
	ret->len = strlen(buff);
	ret->buff = buff;
	return ret;
}

int util_str_equal(UtilString *a, UtilString *b) 
{
	int i;
	int len;

	if (a == b || a->buff == b->buff)
		return true;
	if (a->hash == b->hash && a->len == b->len) {
		len = strlen(a->buff);
		for (i=0; i<len; i++)
			if (a->buff[i] != b->buff[i])
				return false;
		return true;
	}
	return false;
}

int util_str_indexof_(UtilString *a, char *b) {
	if (a == NULL || b == NULL)
		return -1;

	int i,j;
	int blen = strlen(b);
	int alen = a->len - blen;
	if (blen == 0)
		return -1;

	for (i=0; i<alen; i++) {
		bool found = true;
		for (j=0; j<blen; j++) {
			if (a->buff[i+j] != b[j]) {
				found = false;
				break;
			}
		}
		if (found)
			return i;
	}
	return -1;
}

bool util_str_contains_(UtilString *a, char *b) {
	if (a == NULL || b == NULL)
		return false;

	return util_str_indexof_(a, b) >= 0;
}

bool util_str_startswith_(UtilString *a, char *b) {
	if (a == NULL || b == NULL)
		return false;

	return util_str_indexof_(a, b) == 0;
}

