
#include <X11/Xlib.h>

#include <cairo.h>
#include <cairo-xlib.h>

#include <stdio.h>
#include <stdlib.h>

// select and friends
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

// memset
#include <string.h>

#include "gfx/gfx_api.h"
#include "gfx/cairo/cairo.h"


struct container {
	// Xlib stuff
 	Display *dpy;
	int screen;
	Window win;
	struct gfx_callbacks cb;

	// cairo stuff
	cairo_surface_t *cs;

	// some stuff
	int width, height;

	struct gfx_window *delegate_win;
};

static void paint(cairo_surface_t *cs, int w, int h)
{
	cairo_t *c;

	c = cairo_create(cs);
	
	cairo_rectangle(c, 0.0, 0.0, w, h);
	cairo_set_source_rgb(c, 0.0, 0.0, 0.5);
	cairo_fill(c);

	cairo_move_to(c, 10.0, 10.0);
	cairo_set_source_rgb(c, 1.0, 1.0, 0.0);
	cairo_show_text(c, "Hello World!");

	cairo_show_page(c);

	cairo_destroy(c);
}

static void read_event(struct container *c)
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
		paint(c->cs, c->width, c->height);
		break;
	}

	// size events
	case ConfigureNotify: {
		XConfigureEvent *ev = (XConfigureEvent *) &event;
		c->width = ev->width;
		c->height = ev->height;
		if (c->cb.generate_scene)
			c->cb.generate_scene(c->delegate_win);
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
	case ButtonRelease:
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

static void event_handler(struct container *c) 
{
	// http://fixunix.com/xwindows/91558-xconnectionnumber-select.html
	// http://groups.google.com/group/comp.windows.x/browse_thread/thread/cd32588d173fffcf/4b750082b055422b?q=mity&rnum=5&hl=en#4b750082b055422b

	fd_set read;
	int fd = XConnectionNumber(c->dpy);

	FD_ZERO(&read);
	FD_SET(fd, &read);

	XFlush(c->dpy);

	if (!XEventsQueued(c->dpy, QueuedAfterFlush))
		if (select(fd+1, &read, NULL, NULL, NULL) < 0)
			perror("Connection error!");
	read_event(c);
}

static int getW(struct container *c) { return c->width; }
static int getH(struct container *c) { return c->height; }
static struct gfx_callbacks *callbacks(struct container *c) { return &(c->cb); }
static int error_handler(Display *dpy, XErrorEvent *ev)
{
	char buf[256];
	XGetErrorText(dpy, ev->error_code, (char*)&buf, 256);
	printf("XError: %s\n", (char*)&buf);
	return 1;
}


struct gfx_window* gfx_cairo_create_window(int x, int y, int w, int h)
{
 	Display *dpy;
	int screen;
	Window win;
	cairo_surface_t *cs;

	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		fprintf(stderr, "Error: Could not open display\n");
		exit(1);
	}

	XSetErrorHandler(&error_handler);
	screen = DefaultScreen(dpy);
	win = XCreateSimpleWindow(dpy, RootWindow(dpy, screen), 
				  x,y,w,h, 1,
				  BlackPixel(dpy, screen), 
				  WhitePixel(dpy, screen));
	XSelectInput(dpy, win, 
		     MotionNotify |
		     ExposureMask | 
		     KeyPressMask |
		     PropertyChangeMask |
		     StructureNotifyMask |
		     SubstructureNotifyMask |
		     SubstructureRedirectMask |
		     ResizeRedirectMask |
		     FocusChangeMask |
		     VisibilityChangeMask |
		     PropertyChangeMask
		);
	XMapWindow(dpy, win);
	
	cs=cairo_xlib_surface_create(dpy, win, DefaultVisual(dpy, 0), 
				     w, h);

	XSync(dpy, False);

	struct container *c = malloc(sizeof(struct container));
	c->dpy = dpy;
	c->screen = screen;
	c->win = win;
	memset(&(c->cb), 0, sizeof(struct gfx_callbacks));
	c->cs = cs;
	c->width = w;
	c->height = h;
	
	
	struct gfx_window *ret = malloc(sizeof(struct gfx_window));
	ret->impl = c;
	ret->event_handler = (void(*)(void *)) &event_handler;
	ret->callbacks = (struct gfx_callbacks *(*)(void *))&callbacks;
	ret->width = (int(*)(void *))&getW;
	ret->height = (int(*)(void *))&getH;

	c->delegate_win = ret;

	return ret;
}






