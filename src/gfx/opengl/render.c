

#include <GL/gl.h> 

#include "vob/scene.h"
#include "gfx/gfx_api.h"
#include "gfx/opengl/opengl.h"
#include "gfx/opengl/coords.h"
#include "gfx/opengl/vobs.h"
#include "util/dbg.h"


static void pre_render_scene(struct impl *c)
{
	glViewport(0, 0, c->width, c->height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, c->width, c->height, 0, 10000, -10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(drand48(), drand48(), drand48(), 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	//glTranslatef(c->width/2,c->height/2,0);
	//glScalef(0.1f, 0.1f, 1.0f);
}

static void post_render_scene(struct impl *c)
{
	glFlush();
	glXSwapBuffers(c->dpy, c->glxWin);
}

void gfx_opengl_add_opengl_context_task()
{
}

void gfx_opengl_single_render(struct gfx_window *w, Scene *vs)
{
	printf("opengl single render..\n");
	if (vs == NULL)
		return;


	struct impl *c = (struct impl *) w->impl;
	int i;

	pre_render_scene(c);

	// iterate vobs
	printf("iterate vobs %d..\n", vs->vobs_arr->index);
	for (i=0; i<vs->vobs_arr->index; i++) {
		glPushMatrix();

		Vob *v = util_fastarr_get(vs->vobs_arr, i);

		switch (v->type) {
		case VOB0:
			printf("vob0\n");
			break;
		case VOB1: {
			//printf("vob1 %s %p\n", v->id, v);
			Coordsys *cs = util_fastarr_get(vs->coords_arr, i);

			gfx_opengl_vobs_render1(c->id2impl, (Vob1*)v, cs);
			//printf("vob1.. %s\n", v->id);
			break;
		}
		case VOB2:
		default:
			printf("Vob type not implemented.\n");
			break;
		}
		glPopMatrix();
	}
	// draw them



	post_render_scene(c);
}
