// (c) Matti Katila 2012

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#include "lob/models/samemodel.h"
#include "lob/components/limit.h"
#include "vob/coords.h"
#include "util/dbg.h"



static void natsize_event(Lob *this, LobEv *event)
{
	printf("natsize event\n");
	LobNatSize *m = (LobNatSize *) this;
	m->delegate->event(m->delegate, event);
}
static Size *natsize_size(Lob *this)
{
	LobNatSize *m = (LobNatSize *) this;
	return (Size *) &m->size;
}
static Lob *natsize_layout(Lob *this, float w, float h)
{
	LobNatSize *m = (LobNatSize *) this;
	m->delegate = m->delegate->layout(m->delegate, w, h);
	Size *s = m->delegate->size(m->delegate);
	m->size.maxw = s->natw;
	m->size.maxh = s->nath;
	return (Lob*)m;
}
static void natsize_render(Lob *this, Coordsys *into, 
			float w, float h, Scene *vs) 
{
	LobNatSize *m = (LobNatSize *) this;
	m->delegate->render(m->delegate, into, w, h, vs);
} 
Lob *lob_natsize(Region *reg, Lob *delegate)
{
	LobNatSize *ret = REGION(reg, "lob.component.NatSize", LobNatSize);

	ret->base.event = &natsize_event;
	ret->base.size = &natsize_size;
	ret->base.layout = &natsize_layout;
	ret->base.render = &natsize_render;

	ret->delegate = delegate;

	Size *s = ret->delegate->size(ret->delegate);
	ret->size.minw = s->minw;
	ret->size.natw = s->natw;
	ret->size.maxw = s->maxw;
	ret->size.minh = s->minh;
	ret->size.nath = s->nath;
	ret->size.maxh = s->maxh;
	
	return (Lob*)ret;
}


static Size *samew_size(Lob *this)
{
	LobSameW *m = (LobSameW *) this;
	if (m->model->count > 0)
		m->size.natw = m->model->summ / (float) m->model->count;
	return &m->size;
}

Lob *lob_samew(Region *reg, Lob *delegate, LobSameModel *m)
{
	LobSameW *ret = REGION(reg, "lob.component.SameW", LobSameW);
	LobDelegate *d = (LobDelegate *) ret;
	Lob *l = (Lob *) ret;

	l->event = &lob_delegate_event;
	l->size = &samew_size;
	l->layout = &lob_delegate_layout;//&samew_layout;
	l->render = &lob_delegate_render;

	d->delegate = delegate;

	ret->model = m;

	Size *s = ret->base2.delegate->size(ret->base2.delegate);
	ret->size.minw = s->minw;
	ret->size.natw = s->natw;
	lob_samemodel_tick(ret->model, s->natw);
	ret->size.maxw = s->maxw;
	ret->size.minh = s->minh;
	ret->size.nath = s->nath;
	ret->size.maxh = s->maxh;
	
	return (Lob*)ret;

}
