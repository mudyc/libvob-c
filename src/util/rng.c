/* rng.c -- a pseudo-random number generator

  Ported from GNU Classpath 
  (http://developer.classpath.org/doc/java/util/Random-source.html) 
  by Matti Katila 2011.

  Original authors are:
   @author Jochen Hoenicke
   @author Eric Blake (ebb9@email.byu.edu)

  libvob-c is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.

  libvob-c is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with libvob-c; see the file COPYING.  If not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301 USA.
*/


#include <stdio.h>
#include <stdbool.h>

static long long seed = 0;
static long double nextNextGaussian;
static bool haveNextNextGaussian = false;

void util_rng_set_seed(long long seed_) 
{
	seed = (seed_ ^ 0x5DEECE66DLL) & ((1LL << 48) - 1LL);
	haveNextNextGaussian = false;
}


long util_rng_next(int bits)
{
	seed = (seed * 0x5DEECE66DLL + 0xBLL) & ((1LL << 48) - 1LL);
	long next = (seed >> (48 - bits));
	return next;
}
long util_rng_next_int() { return util_rng_next(32); }

long util_rng_next_int_max(long n)
{
//    if (n <= 0)
//	    throw new IllegalArgumentException("n must be positive");
	if ((n & -n) == n) // i.e., n is a power of 2
		return (long) ((n * (long long) util_rng_next(31)) >> 31);
	long bits, val;
	do
	{
		bits = util_rng_next(31);
		val = bits % n;
	}
	while (bits - val + (n - 1) < 0);
	return val;
}

long double util_rng_next_double()
{
	long long a = util_rng_next(26);
	return ((a << 27) + util_rng_next(27)) / (long double) (1LL << 53);
}
bool util_rng_next_boolean()
{
	return util_rng_next(1) != 0;
}

long double util_rng_next_gaussian()
{
	if (haveNextNextGaussian) {
		haveNextNextGaussian = false;
		return nextNextGaussian;
	}
	long double v1, v2, s;
	do {
		v1 = 2 * util_rng_next_double() - 1; // Between -1.0 and 1.0.
		v2 = 2 * util_rng_next_double() - 1; // Between -1.0 and 1.0.
		s = v1 * v1 + v2 * v2;
	} while (s >= 1);
	long double norm = sqrt(-2 * log(s) / s);
	nextNextGaussian = v2 * norm;
	haveNextNextGaussian = true;
	return v1 * norm;
}
