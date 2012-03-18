#include <stdio.h>

#include "lob/api.h"
#include "lob/components/drawing_primitives.h"
#include "vob/vobs.h"
#include "vob/coords.h"

// -------------------------------------------
// Rect
// -------------------------------------------

static void rect_render(Lob *this, Coordsys *into, 
			float w, float h, Scene *vs)
{
	// compose needed vobs from this lob.
	LobRect *r = (LobRect *) this;
	printf("rectrend: %f %f %f \n", r->color->r, r->color->g, r->color->b);
	Vob1 *vob = vob_rect(vs, r->color->r, r->color->g, r->color->b);

	// create a sufficient transformation for the vob
	Coordsys *cs = vob_coords_box(vs, into, 0,0,w, h);

	// bind vob to coordinate system
	vob_scene_put1(vs, vob, cs);
}
static Lob *rect_layout(Lob *lob, float a, float h)
{
	return lob;
}
Lob *lob_rect(Region *reg, VobColor *c_)
{
	LobRect *ret = REGION(reg, "lob.component.Rect", LobRect);
	ret->lob.reg = reg;

	ret->lob.event = &lob_event;
	ret->lob.size = &lob_size;
	ret->lob.layout = &rect_layout;
	ret->lob.render = &rect_render;

	ret->color = c_;
	printf("c %f %f %f\n", ret->color->r, ret->color->g, ret->color->b);
	return (Lob*)ret;
}


// -------------------------------------------
// Rounded Rect
// -------------------------------------------

static void rounded_render(Lob *this, Coordsys *into, 
			float w, float h, Scene *vs)
{
	// compose needed vobs from this lob.
	LobRoundedRect *r = (LobRoundedRect *) this;
	Vob1 *vob = vob_rounded_rect(vs, r->fill, r->t, r->b, r->l, r->r);

	// create a sufficient transformation for the vob
	Coordsys *cs = vob_coords_box(vs, into, 0,0,w, h);

	// bind vob to coordinate system
	vob_scene_put1(vs, vob, cs);
}

Lob *lob_rounded_rect(Region *reg, VobFill *fill, 
		float top, float bottom, float left, float right)
{
	LobRoundedRect *ret = REGION(reg, "lob.component.RoundedRect",
				LobRoundedRect);

	ret->lob.reg = reg;

	ret->lob.event = &lob_event;
	ret->lob.size = &lob_size;
	ret->lob.layout = &lob_layout;
	ret->lob.render = &rounded_render;

	ret->fill = fill;
	ret->t = top;
	ret->b = bottom;
	ret->l = left;
	ret->r = right;
	return (Lob*)ret;
}
