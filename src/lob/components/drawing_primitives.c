
#include "lob/api.h"
#include "lob/components/drawing_primitives.h"
#include "vob/vobs.h"
#include "vob/coords.h"


// -------------------------------------------
// Color
// -------------------------------------------

LobColor *lob_color(Region *reg, float r, float g, float b)
{
	LobColor *ret = REGION(reg, "lob.component.Color", LobColor);
	ret->r = r;
	ret->g = g;
	ret->b = b;
	return ret;
}



// -------------------------------------------
// Rect
// -------------------------------------------

static Lob *rect_layout(Lob *this, int w, int h)
{
	return this;
}

static void rect_render(Lob *this, Coordsys *into, 
			int w, int h, Scene *vs)
{
	// compose needed vobs from this lob.
	LobRect *r = (LobRect *) this;
	Vob1 *vob = vob_rect(vs, r->color->r, r->color->g, r->color->b);

	// create a sufficient transformation for the vob
	Coordsys *cs = vob_coords_box(vs, into, 0,0,w, h);

	// bind vob to coordinate system
	vob_scene_put1(vs, vob, cs);

}

Lob *lob_rect(Region *reg, LobColor *c_)
{
	LobRect *ret = REGION(reg, "lob.component.Rect", LobRect);

	ret->lob.reg = reg;
	ret->lob.size = &lob_size;
	ret->lob.layout = &rect_layout;
	ret->lob.render = &rect_render;

	ret->color = c_;
	return (Lob*)ret;
}
