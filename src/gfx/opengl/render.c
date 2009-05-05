

#include <GL/gl.h> 

#include "vob/scene.h"
#include "gfx/gfx_api.h"
#include "gfx/opengl/opengl.h"
#include "util/dbg.h"


static void pre_render(struct impl *c)
{
	glViewport(0, 0, c->width, c->height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, c->width, c->height, 0, 10000, -10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void post_render(struct impl *c)
{
	glFlush();
	glXSwapBuffers(c->dpy, c->glxWin);
}

void gfx_opengl_single_render(struct window *w, Scene *vs)
{

	struct impl *c = (struct impl *) w->impl;
	int i;

	pre_render(c);

	// iterate vobs
	for (i=0; i<vs->vobs_arr->size; i++) {
		Vob *v = util_fastarr_get(vs->vobs_arr, i);
		switch (v->type) {
		case VOB0:
			printf("vob0\n");
			break;
		case VOB1:
			printf("vob1\n");
			break;
		case VOB2:
		default:
			printf("Vob type not implemented.\n");
			break;
		}
			
	}
	// draw them


	glClearColor(drand48(), drand48(), drand48(), 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	post_render(c);
}
