// (c) Matti Katila 2011, based heavily on code by Benja Fallenstein 2005


#include <stdarg.h>
#include <stdbool.h>


#include "lob/components/spatial_layout.h"
#include "vob/coords.h"

// -------------------------------------------
// Lists - Boxes
// -------------------------------------------

static Size *box_size(Lob *this, bool horiz)
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
			if (horiz) {
				minw += size->minw;
				natw += size->natw;
				maxw += size->maxw;
				minh = fmaxf(minh, size->minh);
				nath = fmaxf(nath, size->nath);
				maxh = fmaxf(maxh, size->maxh);
			} else {
				minh += size->minh;
				nath += size->nath;
				maxh += size->maxh;
				minw = fmaxf(minw, size->minw);
				natw = fmaxf(natw, size->natw);
				maxw = fmaxf(maxw, size->maxw);
			}
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
static Size *vbox_size(Lob *this)
{
	return box_size(this, 0);
}
static Size *hbox_size(Lob *this)
{
	return box_size(this, 1);
}

static Lob *box_layout(Lob *this, float w_, float h_, bool horiz)
{
	LobVBox *box = (LobVBox *) this;
	int i;
	Size *s = box->base.size(this);
	float dH = horiz? w_ - s->natw: h_ - s->nath;
	float totalStretch = horiz? s->maxw - s->natw: s->maxh - s->nath;
	float totalShrink = horiz? s->natw - s->minw: s->nath - s->minh;
	for (i=0; i<box->items->size; i++) {
		Lob *item = util_arr_get(box->items, i);
		Size *is = item->size(item);

		float diff = 0;
		if (dH > 0) { // stretch
			float stretch = horiz? 
				is->maxw - is->natw: is->maxh - is->nath;
			diff = dH * (stretch / totalStretch);
		} else if (dH < 0) { // shrink.. 
			float shrink = horiz?
				is->natw - is->minw: is->nath - is->minh;
			diff = dH * (shrink / totalShrink);
		}

		Lob *lob = item->layout(item, 
					horiz? is->natw+diff: w_, 
					horiz? h_: is->nath + diff);
		if (lob != item)
			util_arr_set(box->items, i, lob);
	}
	return this;
}
static Lob *vbox_layout(Lob *this, float w, float h)
{
	return box_layout(this, w, h, 0);
}
static Lob *hbox_layout(Lob *this, float w, float h)
{
	return box_layout(this, w, h, 1);
}
static void box_render(Lob *this, Coordsys *into, 
		       float w_, float h_, Scene *vs, bool horiz)
{
	// compose needed vobs from this lob.
	LobVBox *box = (LobVBox *) this;

	int i;
	Size *s = box->base.size(this);
	float dH = horiz? w_ - s->natw: h_ - s->nath;
	float totalStretch = horiz? s->maxw - s->natw: s->maxh - s->nath;
	float totalShrink = horiz? s->natw - s->minw: s->nath - s->minh;
	float step = 0;
	for (i=0; i<box->items->size; i++) {
		Lob *item = util_arr_get(box->items, i);
		Size *is = item->size(item);

		float diff = 0;
		if (dH > 0) { // stretch
			float stretch = horiz? 
				is->maxw - is->natw: is->maxh - is->nath;
			diff = dH * (stretch / totalStretch);
		} else if (dH < 0) { // shrink.. 
			float shrink = horiz?
				is->natw - is->minw: is->nath - is->minh;
			diff = dH * (shrink / totalShrink);
		}
		diff = horiz? is->natw + diff: is->nath + diff;
		Coordsys *cs = vob_coords_trans(vs, into, 
						horiz?step:0, 
						horiz?0:step, 0);
		step += diff;
		item->render(item, cs, (horiz?diff: w_), (horiz?h_: diff), vs);
	}
}
static void vbox_render(Lob *this, Coordsys *into, 
			float w, float h, Scene *vs)
{
	box_render(this, into, w, h, vs, 0);
}
static void hbox_render(Lob *this, Coordsys *into, 
			float w, float h, Scene *vs)
{
	box_render(this, into, w, h, vs, 1);
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

Lob *lob_hbox(Region *reg)
{
	LobHBox *ret = REGION(reg, "lob.component.HBox", LobHBox);
	ret->items = util_arr_create(reg);


	ret->base.size = &hbox_size;
	ret->base.layout = &hbox_layout;
	ret->base.render = &hbox_render;

	ret->tmp_size = REGION(reg, "lob.Size", Size);
	ret->tmp_size->minh = -1; // mark

	return ret;
}

void lob_hbox_add(Region *reg, LobHBox *hbox, Lob *lob)
{
	util_arr_add(reg, hbox, lob);
}
