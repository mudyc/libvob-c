// (c): Matti J. Katila

#ifndef UTIL_RNG_H
#define UTIL_RNG_H

#include <stdbool.h>

void util_rng_set_seed(long long seed_);
long util_rng_next(int bits);
long util_rng_next_int();
long util_rng_next_int_max(long n);
long double util_rng_next_double();
bool util_rng_next_boolean();
long double util_rng_next_gaussian();


#endif
