#include "lob/api.h"
#include "vob/api.h"
#include "vob/vobs.h"
#include "vob/coords.h"

#include "util/regions.h"
#include "util/dbg.h"
#include "util/strings.h"


const float LOB_INF = 1073741824.0f;
/*
static void _init_lob_INF() __attribute__ ((constructor));
static void _init_lob_INF()
{
	float *ptr = (float *)&LOB_INF;
	*ptr = (float) powf(2, 30);
}
*/

void lob_render(Lob *l, Scene *sc) 
{
	UNIMPL
}

void lob_event(Lob *l, LobEv *event)
{
	printf("lob event\n");
	// do nothing but dance around the floor.
}
Size *lob_size(Lob *l) 
{
	Size *ret = REGION(l->reg, "lob.Size", Size);

	ret->minw = ret->minh = 0;
	ret->natw = ret->nath = 0;
	ret->maxw = ret->maxh = LOB_INF;

	return ret;
}

Lob *lob_layout(Lob *this, float w, float h)
{
	return this;
}

int lob_min_size() { return 0; }
int lob_pref_size() { return 0; }
int lob_max_size() { return LOB_INF; }


