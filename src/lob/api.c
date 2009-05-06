#include "lob/api.h"
#include "vob/api.h"
#include "vob/vobs.h"
#include "vob/coords.h"

#include "util/regions.h"
#include "util/dbg.h"
#include "util/strings.h"


void lob_render(Lob *l, Scene *sc) 
{
	UNIMPL
}


int lob_min_size() { return 0; }
int lob_pref_size() { return 0; }
int lob_max_size() { return LOB_INF; }




// -------------------------------------------
// Rect
// -------------------------------------------

struct lob_rect {
	Lob lob;
	float r, g, b;
};

static Lob *rect_layout(Lob *this, int w, int h)
{
	return this;
}

static void rect_render(Lob *this, Coordsys *into, 
			int w, int h, Scene *vs)
{
	// compose needed vobs from this lob.
	struct lob_rect *r = (struct lob_rect *) this;
	Vob1 *vob = vob_rect(vs, r->r, r->g, r->b);

	// create a sufficient transformation for the vob
	Coordsys *cs = vob_coords_box(vs, into, w, h);

	// bind vob to coordinate system
	vob_scene_put1(vs, vob, cs);

}

Lob *lob_rect(Region *reg, float r, float g, float b)
{
	REGION_ID(id, "lob.Rect");
	struct lob_rect *ret = (struct lob_rect*) 
		util_regs_instantiate(reg, &id, sizeof(struct lob_rect));

	ret->lob.min_size = &lob_min_size;
	ret->lob.pref_size = &lob_pref_size;
	ret->lob.max_size = &lob_max_size;

	ret->lob.layout = &rect_layout;
	ret->lob.render = &rect_render;

	ret->r = r;
	ret->g = g;
	ret->b = b;
	return (Lob*)ret;
}

