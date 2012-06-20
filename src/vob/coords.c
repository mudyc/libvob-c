#include <stdbool.h>
#include <stdio.h>
#include <err.h>

#include "vob/coords.h"
#include "util/regions.h"
#include "vob/scene.h"
#include "util/dbg.h"


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
	//printf("box2 %f %f %fx%f\n", ret->x,ret->y,ret->w,ret->h);
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
		printf("-trans %f %f %f\n", p->x, p->y, p->z);
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
		return wh(((TransCS*)cs)->parent, w);
	case CS_ORTHO:
		return wh(((OrthoCS*)cs)->parent, w);
	default:
		errx(1, "No width or height implemented for cs: %d\n", 
		     cs->type);
	}
}
float vob_coords_w(Coordsys *cs) {
	float ret = wh(cs, TRUE);
	//printf("ret %f\n",ret);
	return ret;
}
float vob_coords_h(Coordsys *cs) {
	float ret = wh(cs, FALSE);
	//printf("ret %f\n", ret);
	return ret;
}

static void screen2coordsys(float x, float y, 
			Coordsys *cs, float *cs_x, float *cs_y) 
{
	switch (cs->type) {
	case CS_ROOT: {
		*cs_x = x;
		*cs_y = y;
		//DBG("root cs(%f %f)", *cs_x, *cs_y);
		break;
	}
	case CS_BOX: {
		BoxCS *p = (BoxCS*)cs;
		screen2coordsys(x, y, p->parent, cs_x, cs_y);
		*cs_x -= p->x;
		*cs_y -= p->y;
		//DBG("box cs(%f %f)", *cs_x, *cs_y);
		break;
	}
	case CS_TRANS: {
		TransCS *p = (TransCS*)cs;
		screen2coordsys(x, y, p->parent, cs_x, cs_y);
		*cs_x -= p->x;
		*cs_y -= p->y;
		//DBG("trans cs(%f %f)", *cs_x, *cs_y);
		break;
	}
	case CS_ORTHO: {
		OrthoCS *p = (OrthoCS*)cs;
		screen2coordsys(x, y, p->parent, cs_x, cs_y);
		*cs_x -= p->x;
		*cs_y -= p->y;
		//DBG("ortho cs(%f %f)", *cs_x, *cs_y);
		break;
	}
	default:
		errx(1, "No width or height implemented for cs: %d\n", 
		     cs->type);
	}
	
}

bool vob_coords_is_inside(Coordsys *cs, float x, float y)
{
	float w = vob_coords_w(cs);
	float h = vob_coords_h(cs);
	float X,Y;
	screen2coordsys(x, y, cs, &X, &Y);
	//DBG("is inside coords: %f %f", X, Y);
	return 0 <= X && X <= w 
		&& 0 <= Y && Y <= h;
}
