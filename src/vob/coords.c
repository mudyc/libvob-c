
#include "vob/coords.h"
#include "util/regions.h"
#include "vob/scene.h"
#include <stdbool.h>
#include <stdio.h>
#include <err.h>



RootCS *vob_coords_root(Scene *vs)
{
	static char *id = "vob.coords.Root";
	RootCS *ret = (RootCS*) 
		util_regs_instantiate(vs->reg, id, sizeof(RootCS));
	ret->base.type = CS_ROOT;
	return ret;
}

Coordsys *vob_coords_box(Scene *vs, Coordsys *into, 
			 float x, float y, float w, float h) 
{
	BoxCS *ret = REGION(vs->reg, "vob.coords.Box", BoxCS); 
	ret->base.type = CS_BOX;
	ret->parent = into;
	ret->x = x;
	ret->y = y;
	ret->w = w;
	ret->h = h;
	//printf("box %f %f %fx%f\n", x,y,w,h);
	return (Coordsys*) ret;
}

Coordsys *vob_coords_trans(Scene *vs, Coordsys *into, 
			   float x, float y, float z)
{
	TransCS *ret = REGION(vs->reg, "vob.coords.Trans", TransCS); 
	ret->base.type = CS_TRANS;
	ret->parent = into;
	ret->x = x;
	ret->y = y;
	ret->z = z;
	//printf("box %f %f %fx%f\n", x,y,w,h);
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


void vob_coords_dump(Coordsys *cs)
{
	switch (cs->type) {
	case CS_ROOT:
		printf("-root\n");
		break;
	case CS_BOX: {
		BoxCS *p = (BoxCS*)cs;
		printf("-box %f %f %fx%f\n", p->x, p->y, p->w, p->h);
		vob_coords_dump(p->parent);
		break;
	}
	case CS_TRANS: {
		TransCS *p = (TransCS*)cs;
		printf("-trans\n");
		vob_coords_dump(p->parent);
		break;
	}
	case CS_ORTHO:
	default:
		errx(1, "Unknowm coordsys: %d\n", cs->type);
	}	
}

static float wh(Coordsys *cs, bool w) 
{
	switch (cs->type) {
	case CS_ROOT:
		return 1;
	case CS_BOX: {
		BoxCS *p = (BoxCS*)cs;
		return w? p->w: p->h;
	}
	case CS_TRANS:
		return wh(((OrthoCS*)cs)->parent, w);
	case CS_ORTHO:
		return wh(((OrthoCS*)cs)->parent, w);
	default:
		errx(1, "No width or height implemented for cs: %d\n", 
		     cs->type);
	}
}
float vob_coords_w(Coordsys *cs) {
	float ret = wh(cs, TRUE);
	printf("ret %f\n",ret);
	return ret;
}
float vob_coords_h(Coordsys *cs) {
	float ret = wh(cs, FALSE);
	printf("ret %f\n", ret);
	return ret;
}
