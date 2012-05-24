
#include <stdio.h>

#include "lob/components/image.h"
#include "vob/coords.h"


static Size *image_size(Lob *this)
{
	LobImage *m = (LobImage *) this;
	Size *s = &m->size;
	int w, h;
	gfx_image_size(gfx_window_instance(), m->file, &w, &h);
	printf("img size: %d %d\n",w,h);
	m->size.minw = 0;
	m->size.natw = w;
	m->size.maxw = LOB_INF;
	m->size.minh = 0;
	m->size.nath = h;
	m->size.maxh = LOB_INF;
	
	return &m->size;
}
static void image_render(Lob *this, Coordsys *into, 
			float w, float h, Scene *vs) 
{
	LobImage *m = (LobImage *) this;
	Coordsys *cs = vob_coords_box(vs, into, 0,0,w, h);
	printf("render to.. %f %f\n", w, h);
	Vob1 *vob = vob_image(vs, m->file);
	vob_scene_put1(vs, vob, cs);
} 
Lob *lob_image(Region *reg, char *file)
{
	LobImage *ret = REGION(reg, "lob.component.Image", LobImage);

	ret->base.event = &lob_event;
	ret->base.size = &image_size;
	ret->base.layout = &lob_layout;
	ret->base.render = &image_render;

	ret->file = file;

	return (Lob*)ret;
}
