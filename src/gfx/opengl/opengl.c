
#include <X11/Xlib.h>

// Region in Xutil conflicts with our Regions..
#define Region OtherRegion
#include <GL/glx.h> 
#undef Region

#include <GL/gl.h> 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gfx/gfx_api.h"
#include "gfx/animation.h"
#include "gfx/opengl/opengl.h"
#include "gfx/opengl/glyph.h"

#include "gfx/opengl/paper/shaders.h"


void gfx_opengl_single_render(struct gfx_window *w, Scene *vs);

/*
static void paint(Display *dpy, GLXWindow glxWin)
{
	glClearColor(drand48(), drand48(), drand48(), 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glFlush();
    
	glXSwapBuffers(dpy, glxWin);
}
*/

static void read_event(struct impl *c)
{
	XEvent event;
	XNextEvent(c->dpy, &event);

	switch (event.type) {

	// paint events.
	case Expose:
	case GraphicsExpose:
	case ReparentNotify:
	case MapNotify:
	case VisibilityNotify:
	case MapRequest: {
		//paint(c->dpy, c->glxWin);
		gfx_anim_chg(c->delegate_win->anim, CHG_ANIMATE);
		break;
	}

	// size events
	case ConfigureNotify: {
		XConfigureEvent *ev = (XConfigureEvent *) &event;
		c->width = ev->width;
		c->height = ev->height;
		/*
		if (c->cb.generate_scene) {
			c->cb.generate_scene(c->delegate_win);
			paint(c->dpy, c->glxWin);
		}
		*/
		gfx_anim_chg(c->delegate_win->anim, CHG_ANIMATE);
		break;
	}

	// rest are default ones.
	case KeyPress: {
		if (c->cb.key_pressed)
			c->cb.key_pressed();
		break;
	}
	case KeyRelease: {
		if (c->cb.key_released)
			c->cb.key_released();
		break;
	}
	case ButtonPress:
	case ButtonRelease: {
		XButtonEvent me = event.xbutton;
		printf("button: %d,%d %d %d %d\n",
			me.x, me.y, me.state, me.button, me.type);
		LobEv event;
		if (me.type == ButtonPress)
			event.type = LOB_PTR_PRESS;
		else if (me.type == ButtonRelease)
			event.type = LOB_PTR_RELEASE;
		event.x = me.x;
		event.y = me.y;
		event.used = FALSE;
		if (c->cb.event) {
			c->cb.event(c->delegate_win, &event);
		}

		/* XI2 is the way to go
		printf("button codes: %d %d %d %d %p",
			event.xgeneric.extension,
			event.xcookie.extension,
			event.xcookie.evtype,
			event.xcookie.cookie,
			event.xcookie.data);
		*/
		break;
	}
	case MotionNotify:
	case EnterNotify:
	case LeaveNotify:
	case FocusIn:
	case FocusOut:
	case KeymapNotify:
	case NoExpose:
	case CreateNotify:
	case DestroyNotify:
	case UnmapNotify:
	case ConfigureRequest:
	case GravityNotify:
	case ResizeRequest:
	case CirculateNotify:
	case CirculateRequest:
	case PropertyNotify:
	case SelectionClear:
	case SelectionRequest:
	case SelectionNotify:
	case ColormapNotify:
	case ClientMessage:
	case MappingNotify:
	default:
		printf("Un handled.. event! type: %d\n", event.type);
	}

}


static void event_handler(struct impl *c) 
{
	// http://fixunix.com/xwindows/91558-xconnectionnumber-select.html
	// http://groups.google.com/group/comp.windows.x/browse_thread/thread/cd32588d173fffcf/4b750082b055422b?q=mity&rnum=5&hl=en#4b750082b055422b

/*
	fd_set read;
	int fd = XConnectionNumber(c->dpy);

	FD_ZERO(&read);
	FD_SET(fd, &read);

	XFlush(c->dpy);

	if (!XEventsQueued(c->dpy, QueuedAfterFlush))
		if (select(fd+1, &read, NULL, NULL, NULL) < 0)
			perror("Connection error!");
*/
	read_event(c);
}


static int getW(struct impl *c) { return c->width; }
static int getH(struct impl *c) { return c->height; }
static struct gfx_callbacks *callbacks(struct impl *c) { 
	return &(c->cb); 
}
static int getFD(struct impl *c) { return XConnectionNumber(c->dpy); }


static int doubleBufferAttributes[] = {
	GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
	GLX_RENDER_TYPE,   GLX_RGBA_BIT,
	GLX_DOUBLEBUFFER,  True,  /* Request a double-buffered color buffer with */
	GLX_RED_SIZE,      8,     /* the maximum number of bits per component    */
	GLX_GREEN_SIZE,    8, 
	GLX_BLUE_SIZE,     8,
	GLX_ALPHA_SIZE,     8,
	None
};

static Bool WaitForNotify(Display *dpy, XEvent *event, XPointer arg) {
    return (event->type == MapNotify) && (event->xmap.window == (Window) arg);
}

// http://www.opengl.org/sdk/docs/man/xhtml/glXIntro.xml
struct gfx_window* gfx_opengl_create_window(int x, int y, int w, int h)
{
	Display *dpy;
	int screen;
	Window xWin;
	XEvent event;
	
	XVisualInfo *vi;
	XSetWindowAttributes swa;
	int swaMask;

	GLXFBConfig *fbConfigs;
	int fbCount;
	GLXContext cx;
	GLXWindow glxWin;


	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		fprintf(stderr, "Error: Could not open display\n");
		exit(1);
	}

	screen = DefaultScreen(dpy);

	fbConfigs = glXChooseFBConfig(dpy, screen,
				      doubleBufferAttributes, 
				      &fbCount);
	if (fbConfigs == NULL) {
		fprintf(stderr, "Error: could not get double buffered frame buffer\n");
		exit(1);
	}
	vi = glXGetVisualFromFBConfig(dpy, fbConfigs[0]);

	swa.border_pixel = 0;
	swa.event_mask = 
		MotionNotify |
		ExposureMask |
		ButtonPressMask |
		ButtonReleaseMask |
		ButtonMotionMask |
		PointerMotionMask |
		KeyPressMask |
		KeyReleaseMask |
		PropertyChangeMask |
		StructureNotifyMask |
		SubstructureNotifyMask |
		SubstructureRedirectMask |
		ResizeRedirectMask |
		FocusChangeMask |
		VisibilityChangeMask |
		PropertyChangeMask;
	


	swa.colormap = XCreateColormap( dpy, RootWindow(dpy, vi->screen),
					vi->visual, AllocNone );
	
	swaMask = CWBorderPixel | CWColormap | CWEventMask;
	
	xWin = XCreateWindow(dpy, RootWindow(dpy, vi->screen), x,y,w,h,
			      0, vi->depth, InputOutput, vi->visual,
			      swaMask, &swa);

	cx = glXCreateNewContext(dpy, fbConfigs[0], GLX_RGBA_TYPE,
				  NULL, True);
	glxWin = glXCreateWindow(dpy, fbConfigs[0], xWin, NULL);
	
	XMapWindow(dpy, xWin);
	XIfEvent(dpy, &event, WaitForNotify, (XPointer) xWin );

	glXMakeContextCurrent(dpy, glxWin, glxWin, cx);

gfx_opengl_paper_shaders_init();

	glClearColor(1.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glFlush();
    
	glXSwapBuffers(dpy, glxWin);
	
	struct impl *imp = malloc(sizeof(struct impl));
	imp->dpy = dpy;
	imp->glxWin = glxWin;
	memset(&(imp->cb), 0, sizeof(struct gfx_callbacks));
	imp->id2impl = g_hash_table_new(NULL, NULL);

	struct gfx_window *ret = malloc(sizeof(struct gfx_window));
	
	ret->impl = imp;
	imp->delegate_win = ret;

	ret->event_handler = (void(*)(void *)) &event_handler;
	ret->callbacks = (struct gfx_callbacks *(*)(void *)) &callbacks;
	ret->width = (int(*)(void *)) &getW;
	ret->height = (int(*)(void *)) &getH;
	ret->gui_fd = (int(*)(void *)) &getFD;

	ret->font_list = &gfx_opengl_font_list;
	ret->font_glyph_size = &gfx_opengl_font_glyph_size;

	ret->single_render = &gfx_opengl_single_render;

	return ret;
}


