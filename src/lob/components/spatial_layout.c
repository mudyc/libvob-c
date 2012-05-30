// (c) Matti Katila 2011,2012 based heavily on code by Benja Fallenstein 2005

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#include "lob/components/spatial_layout.h"
#include "vob/coords.h"
#include "util/dbg.h"

static Size *glue_size(Lob *this)
{
	LobGlue *g = (LobGlue *) this;
	return &(g->size);
}
static void render_none(Lob *this, Coordsys *into, 
			float w, float h, Scene *vs)
{
	// nil
}
static LobGlue glue = {
	.base.event = &lob_event,
	.base.size = &glue_size,
	.base.layout = &lob_layout,
	.base.render = &render_none,
};
static LobGlue hglue = {
	.base.event = &lob_event,
	.base.size = &glue_size,
	.base.layout = &lob_layout,
	.base.render = &render_none,
};
static LobGlue vglue = {
	.base.event = &lob_event,
	.base.size = &glue_size,
	.base.layout = &lob_layout,
	.base.render = &render_none,
};
static void _init_glues() __attribute__ ((constructor));
static void _init_glues()
{
	glue.size.minw = 0;
	glue.size.natw = 0;
	glue.size.maxw = LOB_INF;
	glue.size.minh = 0;
	glue.size.nath = 0;
	glue.size.maxh = LOB_INF;

	hglue.size.minw = 0;
	hglue.size.natw = 0;
	hglue.size.maxw = LOB_INF;
	hglue.size.minh = -1;
	hglue.size.nath = -1;
	hglue.size.maxh = -1;

	vglue.size.minw = -1;
	vglue.size.natw = -1;
	vglue.size.maxw = -1;
	vglue.size.minh = 0;
	vglue.size.nath = 0;
	vglue.size.maxh = LOB_INF;
}
Lob *lob_glue() {
	return (Lob*)&glue;
}
Lob *lob_hglue() {
	return (Lob*)&hglue;
}
Lob *lob_vglue() {
	Lob *ret = (Lob*)&vglue;
	return ret;
}


// -------------------------------------------
// Lists - Boxes
// -------------------------------------------

static void box_event(Lob *this, LobEv *ev, bool horiz)
{
	printf("box event\n");
	LobBox *box = (LobBox *) this;
}

static void hbox_event(Lob *this, LobEv *ev)
{
	printf("hbox event\n");
	box_event(this, ev, 1);
}
static void vbox_event(Lob *this, LobEv *ev)
{
	printf("vbox event\n");
	box_event(this, ev, 0);
}

static Size *box_size(Lob *this, bool horiz)
{
	LobBox *box = (LobBox *) this;
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
				if (size->minw > 0)
					minw += size->minw;
				if (size->natw > 0)
					natw += size->natw;
				if (size->maxw > 0)
					maxw += size->maxw;
				minh = fmaxf(minh, size->minh);
				nath = fmaxf(nath, size->nath);
				maxh = fmaxf(maxh, size->maxh);
			} else {
				if (size->minh > 0)
					minh += size->minh;
				if (size->nath > 0)
					nath += size->nath;
				if (size->maxh > 0)
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
	LobBox *box = (LobBox *) this;
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
					horiz? is->natw + diff: w_, 
					horiz? h_: is->nath + diff);
		//DBG("%s %f %f", util_regs_dbg(lob), (horiz? is->natw+diff: w_), (horiz? h_: is->nath + diff));
		if (lob != item)
			util_arr_set(box->items, i, lob);
	}
	// invalidate size cache
	box->tmp_size->minh = -1;

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
	LobBox *box = (LobBox *) this;
	// invalidate size cache
	box->tmp_size->minh = -1;
	int i;

	Size *s = box->base.size(this);
	DBG("Box w %f %f  h %f %f", s->natw, s->maxw, s->nath,s->maxh);
	float dH = horiz? w_ - s->natw: h_ - s->nath;
	float totalStretch = horiz? s->maxw - s->natw: s->maxh - s->nath;
	float totalShrink = horiz? s->natw - s->minw: s->nath - s->minh;
	float step = 0;
	for (i=0; i<box->items->size; i++) {
		Lob *item = util_arr_get(box->items, i);
		Size *is = item->size(item);
		DBG("Box %p item %s w %f %f  h %f %f", box, util_regs_dbg(item), is->natw, is->maxw, is->nath,is->maxh);

		float diff = 0;
		if (dH > 0) { // stretch
			float stretch = horiz? 
				is->maxw - is->natw: is->maxh - is->nath;
			diff = dH * (stretch / totalStretch);
			if (isnan(diff))
				diff = dH / box->items->size;;
		} else if (dH < 0) { // shrink.. 
			float shrink = horiz?
				is->natw - is->minw: is->nath - is->minh;
			diff = dH * (shrink / totalShrink);
			if (isnan(diff))
				diff = 0;
		}

		diff = horiz? is->natw + diff: is->nath + diff;
		Coordsys *cs = vob_coords_trans(vs, into, 
						horiz?step:0, 
						horiz?0:step, 0);
		step += diff;
		DBG("%s %f %f", util_regs_dbg(item), (horiz?diff: w_), (horiz?h_: diff));
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
	LobBox *ret = REGION(reg, "lob.component.VBox", LobBox);
	ret->items = util_arr_create(reg);

	ret->base.event = &vbox_event;
	ret->base.size = &vbox_size;
	ret->base.layout = &vbox_layout;
	ret->base.render = &vbox_render;

	ret->tmp_size = REGION(reg, "lob.Size", Size);
	ret->tmp_size->minh = -1; // mark

	return (Lob*)ret;
}

void lob_vbox_add(Region *reg, LobBox *vbox, Lob *lob)
{
	//printf("vbox_add %x \n", lob);
	util_arr_add(reg, vbox->items, lob);
}

Lob *lob_hbox(Region *reg)
{
	LobBox *ret = REGION(reg, "lob.component.HBox", LobBox);
	ret->items = util_arr_create(reg);

	ret->base.event = &hbox_event;
	ret->base.size = &hbox_size;
	ret->base.layout = &hbox_layout;
	ret->base.render = &hbox_render;

	ret->tmp_size = REGION(reg, "lob.Size", Size);
	ret->tmp_size->minh = -1; // mark

	return (Lob*)ret;
}

void lob_hbox_add(Region *reg, LobBox *hbox, Lob *lob)
{
	util_arr_add(reg, hbox->items, lob);
}


static void stack_event(Lob *this, LobEv *ev)
{
	printf("stack event\n");
	LobStack *stack = (LobStack *) this;
	int i;
	for (i=0; i < stack->items->size; i++) {
		Lob *item = util_arr_get(stack->items, i);
		item->event(item, ev);
	}
}
static Size *stack_size(Lob *this)
{
	LobStack *stack = (LobStack *) this;
	int i = stack->items->size;
  
	stack->size.minh = stack->size.maxh = -1;
	stack->size.minw = stack->size.maxw = -1;
	stack->size.nath = stack->size.natw = -1;

	for (i--; i >= 0; i--) {
		Lob *item = util_arr_get(stack->items, i);
		Size *is = item->size(item);
		stack->size.minh = fminf(is->minh, stack->size.minh);
		stack->size.nath = fmaxf(is->nath, stack->size.nath);
		stack->size.maxh = fmaxf(is->maxh, stack->size.maxh);
		stack->size.minw = fminf(is->minw, stack->size.minw);
		stack->size.natw = fmaxf(is->natw, stack->size.natw);
		stack->size.maxw = fmaxf(is->maxw, stack->size.maxw);
	}
	return &stack->size;
}
static Lob *stack_layout(Lob *this, float w, float h)
{
	LobStack *stack = (LobStack *) this;
	int i = stack->items->size;
	for (i--; i >= 0; i--) {
		Lob *item = util_arr_get(stack->items, i);
		Lob *lob = item->layout(item, w, h);
		if (lob != item)
			util_arr_set(stack->items, i, lob);
	}
	return this;
}
static void stack_render(Lob *this, Coordsys *into, 
			float w, float h, Scene *vs)
{
	LobStack *stack = (LobStack *) this;
	int i = stack->items->size;
	for (i--; i >= 0; i--) {
		Coordsys *cs = vob_coords_trans(vs, into, 0, 0, -i);//1/(1+i));
		Lob *item = util_arr_get(stack->items, i);
		item->render(item, cs, w, h, vs);
	}
}
Lob *lob_stack(Region *reg)
{
	LobStack *ret = REGION(reg, "lob.component.Stack", LobStack);
	ret->items = util_arr_create(reg);

	ret->base.event = &stack_event;
	ret->base.size = &stack_size;
	ret->base.layout = &stack_layout;
	ret->base.render = &stack_render;

	return (Lob*)ret;
}
void lob_stack_add(Region *reg, LobStack *stack, Lob *lob)
{
	util_arr_add(reg, stack->items, lob);
}


static void margin_event(Lob *this, LobEv *event)
{
	printf("margin event\n");
	LobMargin *m = (LobMargin *) this;
	m->delegate->event(m->delegate, event);
}
static Size *margin_size(Lob *this)
{
	LobMargin *m = (LobMargin *) this;
	Size *s = m->delegate->size(m->delegate);
	m->size.minw = s->minw + m->l + m->r;
	m->size.natw = s->natw + m->l + m->r;
	m->size.maxw = s->maxw + m->l + m->r;
	m->size.minh = s->minh + m->t + m->b;
	m->size.nath = s->nath + m->t + m->b;
	m->size.maxh = s->maxh + m->t + m->b;
	
	return &m->size;
}
static Lob *margin_layout(Lob *this, float w, float h)
{
	LobMargin *m = (LobMargin *) this;
	m->delegate = m->delegate->layout(m->delegate,
					w - m->l - m->r, h - m->t - m->b);
	return m;
}
static void margin_render(Lob *this, Coordsys *into, 
			float w, float h, Scene *vs) 
{
	LobMargin *m = (LobMargin *) this;
	Coordsys *cs = vob_coords_box(vs, into, m->l, m->t, 
				w - m->l - m->r, h - m->t - m->b);
	m->delegate->render(m->delegate, cs, w - m->l - m->r,
			h - m->t - m->b, vs);
} 
Lob *lob_margin(Region *reg, Lob *delegate, float t, float b, float l, float r)
{
	LobMargin *ret = REGION(reg, "lob.component.Margin", LobMargin);

	ret->base.event = &margin_event;
	ret->base.size = &margin_size;
	ret->base.layout = &margin_layout;
	ret->base.render = &margin_render;

	ret->delegate = delegate;

	ret->t = t;
	ret->b = b;
	ret->l = l;
	ret->r = r;

	return (Lob*)ret;
}
