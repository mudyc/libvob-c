
#include <stdio.h>

#include "lob/components/image.h"
#include "vob/coords.h"


static Size *image_size(Lob *this)
{
	LobImage *m = (LobImage *) this;
	return &m->size;
}
static Lob *image_layout(Lob *this, float w, float h)
{
	LobImage *m = (LobImage *) this;
	if (w > 0 && h > 0) {
		float r = w / h;
		if (r < m->ratio) {// h too much
			m->size.natw = w;
			m->size.nath = w / m->ratio;
			//printf("h too much %f %f %f\n", w, h, m->size.nath);
		} else { // w too much
			//printf("w too much\n");
			m->size.natw = h * m->ratio;
			m->size.nath = h;
		}
	}
	return this;
}
static void image_render(Lob *this, Coordsys *into, 
			float w, float h, Scene *vs) 
{
	LobImage *m = (LobImage *) this;
	Coordsys *cs = vob_coords_box(vs, into, 0,0,w, h);
	Vob1 *vob = vob_image(vs, m->file);
	vob_scene_put1(vs, vob, cs);
} 
Lob *lob_image(Region *reg, char *file)
{
	LobImage *ret = REGION(reg, "lob.component.Image", LobImage);

	ret->base.event = &lob_event;
	ret->base.size = &image_size;
	ret->base.layout = &image_layout;
	ret->base.render = &image_render;

	ret->file = file;

	int w, h;
	gfx_image_size(gfx_window_instance(), file, &w, &h);
	ret->ratio = (w > 0 && h > 0) ? (float)w / (float)h: -1;

	ret->size.minw = 0;
	ret->size.natw = w;
	ret->size.maxw = LOB_INF;
	ret->size.minh = 0;
	ret->size.nath = h;
	ret->size.maxh = LOB_INF;

	return (Lob*)ret;
}
