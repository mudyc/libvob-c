#ifndef GFX_OPENGL_PAPER_COLOR_H
#define GFX_OPENGL_PAPER_COLOR_H

struct color { double r,g,b; };


void gfx_opengl_paper_create_colors(unsigned long seed, int colors, int minlum, 
				    int blend, struct color *ret);



#endif
