#include "lob/composites/button.h"
#include "lob/lobs.h"
#include "vob/vobs.h"



#include <stdlib.h>

static VobColor *c(Region *r, char *red, char *g, char *b) {
	return vob_color(r,
			strtol(red, NULL, 16)/255.,
			strtol(g, NULL, 16)/255.,
			strtol(b, NULL, 16)/255.);
}

static Lob *lob(Region *r)
{
	VobColor *color = vob_color(r, .9, .2, .5);
	    //print 'color', color

	//VobShade *vob_shade(Region *reg, enum SHADE_TYPE type, float angle);
	//void vob_shade_add(Region *reg, VobShade *shade, float step, VobColor *c);

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
	LobBox *l = lob_vbox(r);
	UtilArray *fonts = lob_font_list(r);
	LobBox *text =
		lob_font_text(r, util_arr_get(fonts, 12), 12, "", color); 
	lob_hbox_add(r, text, lob_hglue()); 

	lob_vbox_add(r, (LobBox *)l, lob_vglue()); 
	lob_vbox_add(r, (LobBox *)l, (Lob*)text); 
	lob_vbox_add(r, (LobBox *)l, lob_vglue()); 

	LobStack *ret = lob_stack(r);
	lob_stack_add(r, (LobStack *)ret, l);
	lob_stack_add(r, (LobStack *)ret, center);
	lob_stack_add(r, (LobStack *)ret, edge);
	printf("lob done \n");
	return ret;
}
static void button_event(Lob *this, LobEv *event)
{
	printf("button event\n");
	LobButton *m = (LobButton *) this;
	m->delegate->event(m->delegate, event);
}
static Size *button_size(Lob *this)
{
	printf("button size\n");
	LobButton *m = (LobButton *) this;
	Size *s = m->delegate->size(m->delegate);
	return s;
}
static Lob *button_layout(Lob *this, float w, float h)
{
	printf("button layout\n");
	LobButton *m = (LobButton *) this;
	m->delegate = m->delegate->layout(m->delegate, w, h);
	return m;
}
static void button_render(Lob *this, Coordsys *into, 
			float w, float h, Scene *vs) 
{
	LobButton *m = (LobButton *) this;
	m->delegate->render(m->delegate, into, w, h, vs);
} 
Lob *lob_button(Region *reg, char *text, LobClickModel *click)
{
	LobButton *ret = REGION(reg, "lob.composite.Button", LobButton);

	ret->base.event = &button_event;
	ret->base.size = &button_size;
	ret->base.layout = &button_layout;
	ret->base.render = &button_render;

	ret->delegate = lob(reg);

	printf("button: %p\n",ret);
	return (Lob*)ret;
}
