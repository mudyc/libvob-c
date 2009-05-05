#ifndef GFX_OPENGL_OPENGL_H
#define GFX_OPENGL_OPENGL_H

#include <X11/Xlib.h>

// Region in Xutil conflicts with our Regions..
#define Region OtherRegion
#include <GL/glx.h> 
#undef Region

#include "gfx/gfx_api.h"

// for opengl/render
struct impl {
	Display *dpy;
	Window win;

	GLXContext cx;
	GLXWindow glxWin;
	
	int width;
	int height;
	struct gfx_callbacks cb;
	struct window *delegate_win;
};



struct window* gfx_opengl_create_window(int x, int y, int w, int h);




#endif
