
#include "util/primitives.h"


UtilF *util_float(Region *reg, float value)
{
	UtilF *ret = REGION(reg, "util.Float", UtilF);
	ret->value = value;
	return ret;
}
