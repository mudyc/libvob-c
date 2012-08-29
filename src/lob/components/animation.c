
#include "lob/components/animation.h"
#include "vob/coords.h"

static void key_render(Lob *this, Coordsys *into, 
			float w, float h, Scene *vs)
{
	LobKey *k = (LobKey *) this;
	into = vob_coords_box(vs, into, 0,0, w,h);
	vs->matcher->add(vs->matcher, into, k->key);
	lob_delegate_render(this, into, w, h, vs);
}
Lob *lob_key(Region *reg, Lob *delegate, char *key)
{
	LobKey *ret = REGION(reg, "lob.component.Key", LobKey);
	LobDelegate *ret_d = (LobDelegate *) ret;
	Lob *ret_l = (Lob *) ret;

	ret_l->event = &lob_delegate_event;
	ret_l->size = &lob_delegate_size;
	ret_l->layout = &lob_delegate_layout;
	ret_l->render = &key_render;

	ret_d->delegate = delegate;

	ret->key = key;

	return (Lob*)ret;
}

static void key_anim_render(Lob *this, Coordsys *into, 
			float w, float h, Scene *vs)
{
	LobKeyAnim *k = (LobKeyAnim *) this;
	LobDelegate *d = (LobDelegate *) this;
	//vs->matcher->add(vs->matcher, into, k->key);

	Coordsys *cs = vs->previous->matcher->get(vs->previous->matcher, k->key);
	into = vob_coords_box(vs, into, 0,0,w, h);
	if (cs != NULL) {
		//lob_keyanimmodel_init(k->model, cs, to);

		lob_keyanimmodel_render(k->model, d->delegate, 
					cs, into, w, h, vs);
	}
	else 
		lob_delegate_render(this, into, w, h, vs);

	if (k->make_key)
		vs->matcher->add(vs->matcher, into, k->key);
}

Lob *lob_key_anim(Region *reg, Lob *delegate, char *key, bool make_key, LobKeyAnimModel *m)
{
	LobKeyAnim *ret = REGION(reg, "lob.component.KeyAnim", LobKeyAnim);
	LobDelegate *ret_d = (LobDelegate *) ret;
	Lob *ret_l = (Lob *) ret;

	ret_l->event = &lob_delegate_event;
	ret_l->size = &lob_delegate_size;
	ret_l->layout = &lob_delegate_layout;
	ret_l->render = &key_anim_render;

	ret_d->delegate = delegate;

	ret->key = key;
	ret->model = m;
	ret->make_key = make_key;

	return (Lob*)ret;
}
