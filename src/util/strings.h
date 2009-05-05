// (c): Matti J. Katila

#ifndef UTILS_STRINGS_H
#define UTILS_STRINGS_H

#include <stdbool.h>

/** String utils for fast hashing etc.
 */


struct util_string {
	int hash;
	int len;
	// UTF-8
	char *buff;
};
typedef struct util_string UtilString;

int util_str_equal(UtilString *a, UtilString *b);

UtilString *util_str_create(char *buff);

int util_str_hash(UtilString *s);

bool util_str_contains_(UtilString *s, char *b);

bool util_str_startswith_(UtilString *s, char *b);

#endif
