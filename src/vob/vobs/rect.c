
#include "vob/api.h"
#include "vob/vobs/rect.h"
#include "util/regions.h"




Vob1 *vob_rect(Scene *vs, float r, float g, float b)
{
	REGION_ID(id, "vob.vobs.Rect");
	Rect *ret = (Rect*) 
		util_regs_instantiate(vs->reg, &id, sizeof(Rect));

	ret->base.type = VOB1;
	ret->base.id = id.id;

	ret->r = r;
	ret->g = g;
	ret->b = b;

	return (Vob1*)ret;
}
