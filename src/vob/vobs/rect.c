
#include "vob/api.h"
#include "vob/vobs/rect.h"
#include "util/regions.h"




Vob1 *vob_rect(Scene *vs, float r, float g, float b)
{
	static char *id = "vob.vobs.Rect";
	Rect *ret = (Rect*) 
		util_regs_instantiate(vs->reg, id, sizeof(Rect));

	ret->base.type = VOB1;
	ret->base.id = id;

	ret->r = r;
	ret->g = g;
	ret->b = b;
	printf("%f %f %f\n", ret->r, ret->g, ret->b);
	return (Vob1*)ret;
}

Vob1 *vob_rounded_rect(Scene *vs, VobFill *fill)
{
	static char *id = "vob.vobs.RoundedRect";
	VobRoundedRect *ret = (VobRoundedRect*) 
		util_regs_instantiate(vs->reg, id, sizeof(VobRoundedRect));

	ret->base.type = VOB1;
	ret->base.id = id;

	ret->fill = fill;

	return (Vob1*)ret;

}
