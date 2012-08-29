#include "lob/composites/button.h"
#include "lob/lobs.h"
#include "vob/vobs.h"
#include "vob/coords.h"
#include "gfx/gfx_api.h"
#include "gfx/animation.h"


#include <stdlib.h>

static VobColor *c(Region *r, char *red, char *g, char *b) {
	return vob_color(r,
			strtol(red, NULL, 16)/255.,
			strtol(g, NULL, 16)/255.,
			strtol(b, NULL, 16)/255.);
}

static Lob *lob(Region *r, Lob *text)
{

	VobShade *shade = vob_shade(r, 1, 0); 
	vob_shade_add(r, shade,  0, c(r, "58","59","5b"));
	vob_shade_add(r, shade,  0.22, c(r, "a7","a7","a7"));
	vob_shade_add(r, shade,  0.45, c(r, "45","45","47"));
	vob_shade_add(r, shade,  0.60, c(r, "fe","fe","fe"));
	vob_shade_add(r, shade,  0.8, c(r, "ad","ad","ad"));
	vob_shade_add(r, shade,  1, c(r, "fa","fa","fa"));

	Lob *edge = lob_rounded_rect(r, shade, 20.,20.,20.,20.);

	VobShade *shade2 = vob_shade(r, 1, 0); 
	vob_shade_add(r, shade2,  0, c(r, "58","59","5b"));
	vob_shade_add(r, shade2,  0.6, c(r, "fe","fe","fe"));
	vob_shade_add(r, shade2,  1, c(r, "53","52","5b"));
	Lob *center = lob_margin(r, 
				lob_rounded_rect(r, shade2, 20.,20.,20.,20.),
                                10,10,10,10);
	LobStack *ret = lob_stack(r);
	lob_stack_add(r, (LobStack *)ret, text);
	lob_stack_add(r, (LobStack *)ret, center);
	lob_stack_add(r, (LobStack *)ret, edge);
	//printf("lob done %x\n", ret);
	return ret;
}

static Lob *lob_pressed(Region *r, Lob *text)
{

	VobShade *shade = vob_shade(r, 1, 0); 
	vob_shade_add(r, shade,  0, c(r, "58","59","5b"));
	vob_shade_add(r, shade,  0.22, c(r, "a7","a7","a7"));
	vob_shade_add(r, shade,  0.45, c(r, "45","45","47"));
	vob_shade_add(r, shade,  0.60, c(r, "fe","fe","fe"));
	vob_shade_add(r, shade,  0.8, c(r, "ad","ad","ad"));
	vob_shade_add(r, shade,  1, c(r, "fa","fa","fa"));

	Lob *edge = lob_rounded_rect(r, shade, 20.,20.,20.,20.);

	VobShade *shade2 = vob_shade(r, 1, 0); 
	vob_shade_add(r, shade2,  0, c(r, "fe","fe","fe"));
	vob_shade_add(r, shade2,  0.4, c(r, "58","59","5b"));
	vob_shade_add(r, shade2,  1, c(r, "fe","fe","fe"));
	Lob *center = lob_margin(r, 
				lob_rounded_rect(r, shade2, 20.,20.,20.,20.),
                                10,10,10,10);
	LobStack *ret = lob_stack(r);
	lob_stack_add(r, (LobStack *)ret, text);
	lob_stack_add(r, (LobStack *)ret, center);
	lob_stack_add(r, (LobStack *)ret, edge);
	//printf("lob done %x\n", ret);
	return ret;
}

static Lob *text_lob(Region *r, char *text) {

	LobBox *l = lob_vbox(r);

	VobColor *color = vob_color(r, .9, .2, .5);

	UtilArray *fonts = lob_font_list(r);
	LobBox *text_lob =
		lob_font_text(r, util_arr_get(fonts, 14), 22, text, color); 
	LobBox *t = lob_hbox(r);
	lob_hbox_add(r, t, lob_hglue()); 
	lob_hbox_add(r, t, text_lob); 
	lob_hbox_add(r, t, lob_hglue()); 

	lob_vbox_add(r, (LobBox *)l, lob_vglue()); 
	lob_vbox_add(r, (LobBox *)l, (Lob*)t); 
	lob_vbox_add(r, (LobBox *)l, lob_vglue());

	return (Lob *)l;
}

static void button_event(Lob *this, LobEv *event)
{
	LobButton *m = (LobButton *) this;
	Lob *lob = (Lob *)m;
	//printf("button event %p\n", lob->cs);
	if (vob_coords_is_inside(lob->cs, (float)event->x, (float)event->y)) {
		struct gfx_window *win = gfx_window_instance();
		if (event->type == LOB_PTR_PRESS) {
			if (lob_clickmodel_set(m->model, PRESSED)
			    && !gfx_anim_is_chg_incoming(win->anim))
				gfx_anim_chg(win->anim, 
					CHG_SWITCH_VOB_SCENE);
		}
		if (event->type == LOB_PTR_RELEASE) {
			if (lob_clickmodel_set(m->model, RELEASED)
			    && !gfx_anim_is_chg_incoming(win->anim))
				gfx_anim_chg(win->anim, 
					CHG_SWITCH_VOB_SCENE);
		}
		m->delegate->event(m->delegate, event);
	}
}
static Size *button_size(Lob *this)
{
	LobButton *m = (LobButton *) this;
	Size *s = m->delegate->size(m->delegate);
	return s;
}
static Lob *button_layout(Lob *this, float w, float h)
{
	LobButton *m = (LobButton *) this;
	m->delegate = m->delegate->layout(m->delegate, w, h);
	return m;
}
static void button_render(Lob *this, Coordsys *into, 
			float w, float h, Scene *vs) 
{
	LobButton *m = (LobButton *) this;
	Lob *lob = (Lob *)m;
	lob->cs = vob_coords_box(vs, into, 0, 0, w, h);
	m->delegate->render(m->delegate, into, w, h, vs);
} 
Lob *lob_button(Region *reg, char *text, LobClickModel *click)
{
	LobButton *ret = REGION(reg, "lob.composite.Button", LobButton);

	ret->base.event = &button_event;
	ret->base.size = &button_size;
	ret->base.layout = &button_layout;
	ret->base.render = &button_render;

	ret->model = click;

	
	if (click->state == RELEASED)
		ret->delegate = lob(reg, text_lob(reg, text));
	else
		ret->delegate = lob_pressed(reg, text_lob(reg, text));

	//printf("button: %p %p %d\n",ret, click, click->state);
	return (Lob*)ret;
}
