#ifndef UTIL_PRIMITIVES_H
#define UTIL_PRIMITIVES_H

#include "util/regions.h"

struct util_float {
	float value;
};
typedef struct util_float UtilF;

UtilF *util_float(Region *reg, float value);

#endif
