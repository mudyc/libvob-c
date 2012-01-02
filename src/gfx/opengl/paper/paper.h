#ifndef GFX_OPENGL_PAPER_PAPER_H
#define GFX_OPENGL_PAPER_PAPER_H

#include <GL/gl.h>
#include "vob/api.h"
#include "gfx/opengl/paper/color.h"

struct gfx_opengl_paper_pass_ {
	long seed;
	double transparency;
	//struct color colors[8]; // 8 items.
	GLuint textures[2]; // 2 texgens
	double textures_scale[2]; // 2 texgens

	int path;
	//double rndscale;
	double scale, band_or_alphascale;

	// texcoords
	double texgenrep[2][2][4]; // 2 texgen per pass, 3?x4 matrix
};
typedef struct gfx_opengl_paper_pass_ GfxPaperPass;


struct gfx_opengl_paper_ {
	unsigned pass_count;

	struct color colors[8]; // 8 items.
	int tex_probs[20]; // there are textures less but these are just propabilities.

	GfxPaperPass pass[3];

	// texcoords
	double rootrep[2][2];
};
typedef struct gfx_opengl_paper_ GfxPaper;

GfxPaper *gfx_opengl_paper_create(unsigned long seed);

void gfx_opengl_paper(Vob1 *v, Coordsys *cs);


#endif
