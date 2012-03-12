#include <string.h>
#include <stdio.h>
#include "gfx/opengl/coords.h"
#include "gfx/opengl/vobs.h"
#include "gfx/opengl/glyph.h"
#include "gfx/opengl/paper/paper.h"
#include "vob/coords.h"
#include "vob/vobs/rect.h"
#include <GL/gl.h> 

// prototype here of rounded rect.
void gfx_opengl_rounded_rect(VobRoundedRect *r, Coordsys *cs);


static void rect(Rect *r, Coordsys *cs) {
	glColor3f(r->r, r->g, r->b);
	
	float 
		w = vob_coords_w(cs),
		h = vob_coords_h(cs);
	printf("rect: %2.1fx%2.1f %0.2f,%0.2f,%0.2f\n", w,h, r->r, r->g, r->b);

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
		else if (strcmp(key, "vob.vobs.Glyph") == 0)
			value = &gfx_opengl_glyph;
		else if (strcmp(key, "vob.vobs.Paper") == 0)
			value = &gfx_opengl_paper;
		else if (strcmp(key, "vob.vobs.RoundedRect") == 0)
			value = &gfx_opengl_rounded_rect;

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
