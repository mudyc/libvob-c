// (c) Matti Katila 2011, based heavily on code by Benja Fallenstein 2005


#include <stdarg.h>


#include "lob/components/spatial_layout.h"
#include "vob/coords.h"

// -------------------------------------------
// Lists - Boxes
// -------------------------------------------

static Size *vbox_size(Lob *this)
{
	LobVBox *box = (LobVBox *) this;
	if (box->tmp_size->minh < 0) {
		int i;
		float minw,natw,maxw;
		float minh,nath,maxh;
		minw=natw=maxw = 0;
		minh=nath=maxh = 0;
		for (i=0; i<box->items->size; i++) {
			Lob *item = (Lob *)util_arr_get(box->items, i);
			Size *size = item->size(item);
			minh += size->minh;
			nath += size->nath;
			maxh += size->maxh;
			minw = fmaxf(minw, size->minw);
			natw = fmaxf(natw, size->natw);
			maxw = fmaxf(maxw, size->maxw);
		}
		Size *s = box->tmp_size;
		s->minw = minw;
		s->natw = natw;
		s->maxw = maxw;
		s->minh = minh;
		s->nath = nath;
		s->maxh = maxh;
	}
	
	return box->tmp_size;
}

static Lob *vbox_layout(Lob *this, int w, int h)
{
	LobVBox *box = (LobVBox *) this;
	int i;
	Size *s = box->base.size(this);
	float dH = h - s->nath;
	float totalStretch = s->maxh - s->nath;
	float totalShrink = s->nath - s->minh;
	for (i=0; i<box->items->size; i++) {
		Lob *item = util_arr_get(box->items, i);
		Size *is = item->size(item);

		float diff = 0;
		if (dH > 0) { // stretch
			float stretch = is->maxh - is->nath;
			diff = dH * (stretch / totalStretch);
		} else if (dH < 0) { // shrink.. 
			float shrink = is->nath - is->minh;
			diff = dH * (shrink / totalShrink);
		}
		Lob *lob = item->layout(item, w, is->nath + diff);
		if (lob != item)
			util_arr_set(box->items, i, lob);
	}
	return this;
}

static void vbox_render(Lob *this, Coordsys *into, 
			int w, int h_, Scene *vs)
{
	printf("vbox_render\n");
	// compose needed vobs from this lob.
	LobVBox *box = (LobVBox *) this;

	int i;
	Size *s = box->base.size(this);
	float dH = h_ - s->nath;
	float totalStretch = s->maxh - s->nath;
	float totalShrink = s->nath - s->minh;
	float y = 0;
	for (i=0; i<box->items->size; i++) {
		Lob *item = util_arr_get(box->items, i);
		Size *is = item->size(item);

		float diff = 0;
		if (dH > 0) { // stretch
			float stretch = is->maxh - is->nath;
			diff = dH * (stretch / totalStretch);
		} else if (dH < 0) { // shrink.. 
			float shrink = is->nath - is->minh;
			diff = dH * (shrink / totalShrink);
		}
		float h = is->nath + diff;
		Coordsys *cs = vob_coords_box(vs, into, 0, y, w, h);
		y += h;
		printf("render %d\n", i);
		item->render(item, cs, w, h, vs);
	}
}

Lob *lob_vbox(Region *reg)
{
	LobVBox *ret = REGION(reg, "lob.component.VBox", LobVBox);
	ret->items = util_arr_create(reg);


	ret->base.size = &vbox_size;
	ret->base.layout = &vbox_layout;
	ret->base.render = &vbox_render;

	ret->tmp_size = REGION(reg, "lob.Size", Size);
	ret->tmp_size->minh = -1; // mark

	return ret;
}

void lob_vbox_add(Region *reg, LobVBox *vbox, Lob *lob)
{
	util_arr_add(reg, vbox, lob);
}
