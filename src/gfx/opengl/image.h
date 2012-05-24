#ifndef GFX_OPENGL_IMAGE_H
#define GFX_OPENGL_IMAGE_H

#include "vob/vobs/image.h"

void gfx_opengl_image_size(char *file, int *w, int *h);
void gfx_opengl_image(Image *i, Coordsys *cs);

#endif
