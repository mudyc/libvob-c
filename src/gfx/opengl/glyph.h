#ifndef GFX_OPENGL_GLYPH
#define GFX_OPENGL_GLYPH

// (c): Matti Katila, 2011

#include "vob/api.h"

// render function
void gfx_opengl_glyph(Vob1 *v, Coordsys *cs);

void gfx_opengl_font_list(int *length, char ***list);
void gfx_opengl_font_glyph_size(void *font_ptr, char *ch, 
				float size, float *w, float *h);

#endif
