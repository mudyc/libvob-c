#ifndef VOB_MATCHER_H
#define VOB_MATCHER_H

//! @nopython

#include "vob/coords.h"
#include "util/regions.h"
struct vob_matcher;
typedef struct vob_matcher VobMatcher;
struct vob_matcher {
	GHashTable *cs2key;
	GHashTable *key2cs;
	void (*add)(VobMatcher *m, Coordsys *cs, char *key);

	Coordsys *(*get)(VobMatcher *m, char *key);
	
	Coordsys *(*animationCS)(Region *reg, Coordsys *cs, 
				VobMatcher *m0, VobMatcher *m1, float fract);
};

VobMatcher *vob_matcher(Region *r);
void vob_matcher_clear(VobMatcher *m);

#endif
