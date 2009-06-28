
#include "vob/coords.h"
#include "util/regions.h"
#include "vob/scene.h"
#include <stdbool.h>
#include <err.h>

RootCS *vob_coords_root(Scene *vs)
{
	static char *id = "vob.coords.Root";
	RootCS *ret = (RootCS*) 
		util_regs_instantiate(vs->reg, id, sizeof(RootCS));
	ret->base.type = CS_ROOT;
	return ret;
}

Coordsys *vob_coords_box(Scene *vs, Coordsys *into, float w, float h) 
{
	static char *id = "vob.coords.Box";
	BoxCS *ret = (BoxCS*) 
		util_regs_instantiate(vs->reg, id, sizeof(BoxCS));
	ret->base.type = CS_BOX;
	ret->parent = into;
	ret->w = w;
	ret->h = h;
	//into->add(vs, ret);
	return (Coordsys*) ret;
}

Coordsys *vob_coords_ortho(Scene *vs, Coordsys *into,
			   float x, float y, float z,
			   float sx, float sy)
{
	static char *id = "vob.coords.Ortho";
	OrthoCS *ret = (OrthoCS*)
		util_regs_instantiate(vs->reg, id, sizeof(OrthoCS));
	ret->base.type = CS_ORTHO;
	ret->parent = into;
	ret->x = x;
	ret->y = y;
	ret->z = z;
	ret->sx = sx;
	ret->sy = sy;
	return (Coordsys*) ret;
}


static float wh(Coordsys *cs, bool w) 
{
	switch (cs->type) {
	case CS_ROOT:
		return 1;
	case CS_BOX: {
		BoxCS *p = (BoxCS*)cs;
		return w?p->w:p->h;
	}
	case CS_ORTHO:
		return wh(((OrthoCS*)cs)->parent, w);
	default:
		errx(1, "No width or height implemented for cs: %d\n", 
		     cs->type);
	}
}
float vob_coords_w(Coordsys *cs) {
	return wh(cs, TRUE);
}
float vob_coords_h(Coordsys *cs) {
	return wh(cs, FALSE);
}
