#include <string.h>
#include <stdio.h>
#include "gfx/opengl/coords.h"
#include "gfx/opengl/vobs.h"
#include "vob/coords.h"
#include "vob/vobs/rect.h"
#include <GL/gl.h> 


static void rect(Rect *r, Coordsys *cs) {
	glColor3f(r->r, r->g, r->b);
	
	float 
		w = vob_coords_w(cs),
		h = vob_coords_h(cs);

	glBegin(GL_QUADS);
	glVertex2f(0, h);
	glVertex2f(w, h);
	glVertex2f(w, 0);
	glVertex2f(0, 0);
	glEnd();
}


void gfx_opengl_vobs_render1(GHashTable *id2impl, 
			     Vob1 *v, Coordsys *cs)
{
	gpointer key, value;
	key = v->base.id;
	value = g_hash_table_lookup(id2impl, key);
	
	

	if (value == NULL) {
		if (strcmp(key, "vob.vobs.Rect") == 0)
			value = &rect;
		

		if (value != NULL)
			g_hash_table_insert(id2impl, key, value);
		else {
			printf("No implementation for %s", (char *)key);
			abort();
		}
	}

	if (value != NULL && cs != NULL) {
		gfx_opengl_coords(cs);
		void (*func)(Vob1 *v, Coordsys *cs);
		func = value;
		func(v, cs);
	}
}
