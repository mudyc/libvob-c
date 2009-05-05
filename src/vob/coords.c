
#include "vob/coords.h"
#include "util/regions.h"
#include "vob/scene.h"


RootCS *vob_coords_root(Scene *vs)
{
	REGION_ID(id, "vob.coords.Root");

	RootCS *ret = (RootCS*) 
		util_regs_instantiate(vs->reg, &id, sizeof(RootCS));
	ret->base.type = CS_ROOT;
	return ret;
}

Coordsys *vob_coords_box(Scene *vs, Coordsys *into, float w, float h) 
{
	REGION_ID(id, "vob.coords.Box");
	BoxCS *ret = (BoxCS*) 
		util_regs_instantiate(vs->reg, &id, sizeof(BoxCS));
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
	REGION_ID(id, "vob.coords.Ortho");
	OrthoCS *ret = (OrthoCS*)
		util_regs_instantiate(vs->reg, &id, sizeof(OrthoCS));
	ret->base.type = CS_BOX;
	ret->parent = into;
	ret->x = x;
	ret->y = y;
	ret->z = z;
	ret->sx = sx;
	ret->sy = sy;
	return (Coordsys*) ret;
}
