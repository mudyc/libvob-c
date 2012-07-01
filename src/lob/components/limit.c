// (c) Matti Katila 2012

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#include "lob/components/limit.h"
#include "lob/models/samemodel.h"
#include "vob/coords.h"
#include "util/dbg.h"



static Size *natsize_size(Lob *this)
{
	LobNatSize *m = (LobNatSize *) this;
	return (Size *) &m->size;
}
static Lob *natsize_layout(Lob *this, float w, float h)
{
	LobNatSize *nat = (LobNatSize *) this;
	LobDelegate *nat_d = (LobDelegate *) nat;
	nat_d->delegate = nat_d->delegate->layout(nat_d->delegate, w, h);
	Size *s = nat_d->delegate->size(nat_d->delegate);
	nat->size.natw = s->natw;
	nat->size.nath = s->nath;
	nat->size.maxw = s->natw;
	nat->size.maxh = s->nath;
	return (Lob*)nat;
}
Lob *lob_natsize(Region *reg, Lob *delegate)
{
	LobNatSize *ret = REGION(reg, "lob.component.NatSize", LobNatSize);
	LobDelegate *ret_d = (LobDelegate *) ret;
	Lob *ret_l = (Lob *) ret;

	ret_l->event = &lob_delegate_event;
	ret_l->size = &natsize_size;
	ret_l->layout = &natsize_layout;
	ret_l->render = &lob_delegate_render;

	ret_d->delegate = delegate;

	Size *s = ret_d->delegate->size(ret_d->delegate);
	ret->size.minw = s->minw;
	ret->size.natw = s->natw;
	ret->size.maxw = s->maxw;
	ret->size.minh = s->minh;
	ret->size.nath = s->nath;
	ret->size.maxh = s->maxh;
	
	return (Lob*)ret;
}

static Size *reqsize_size(Lob *this)
{
	LobReqSize *r = (LobReqSize *) this;
	return r->size;
}
Lob *lob_reqsize(Region *reg, Lob *delegate, Size *s)
{
	LobReqSize *ret = REGION(reg, "lob.component.ReqSize", LobReqSize);
	LobDelegate *ret_d = (LobDelegate *) ret;
	Lob *ret_l = (Lob *) ret;

	ret_l->event = &lob_delegate_event;
	ret_l->size = &reqsize_size;
	ret_l->layout = &lob_delegate_layout;
	ret_l->render = &lob_delegate_render;

	ret_d->delegate = delegate;

	ret->size = s;

	return (Lob*)ret;
}


static Size *samew_size(Lob *this)
{
	LobSameW *same = (LobSameW *) this;
	LobDelegate *d = (LobDelegate *) same;
	Size *s = d->delegate->size(d->delegate);
	same->size.minw = s->minw;
	same->size.natw = same->model->summ / (float) same->model->count;
	same->size.maxw = s->maxw;
	same->size.minh = s->minh;
	same->size.nath = s->nath;
	same->size.maxh = s->maxh;
	return (Size *)&same->size;
}

Lob *lob_samew(Region *reg, Lob *delegate, LobSameModel *m)
{
	LobSameW *ret = REGION(reg, "lob.component.SameW", LobSameW);
	LobDelegate *d = (LobDelegate *) ret;
	Lob *l = (Lob *) ret;

	l->event = &lob_delegate_event;
	l->size = &samew_size;
	l->layout = &lob_delegate_layout;
	l->render = &lob_delegate_render;

	d->delegate = delegate;

	ret->model = m;

	Size *s = d->delegate->size(d->delegate);
	ret->size.minw = s->minw;
	ret->size.natw = s->natw;
	lob_samemodel_tick(ret->model, s->natw);
	ret->size.maxw = s->maxw;
	ret->size.minh = s->minh;
	ret->size.nath = s->nath;
	ret->size.maxh = s->maxh;
	
	return (Lob*)ret;

}
