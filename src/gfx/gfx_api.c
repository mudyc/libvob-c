
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "gfx/gfx_api.h"
#include "gfx/animation.h"
#include "gfx/render.h"
#include "lob/main.h"
#include "util/dbg.h"

#if GFX_API_CAIRO
#include "gfx/cairo/cairo.h"
#endif

#if GFX_API_OPENGL
#include "gfx/opengl/opengl.h"
#endif


int gfx_height(struct gfx_window *w) 
{
	return w->height(w->impl);
}

int gfx_width(struct gfx_window *w) 
{
	return w->width(w->impl);
}

void gfx_font_list(struct gfx_window *w,
		   int *length, char ***list) 
{
	w->font_list(length, list);
}
void gfx_font_glyph_size(struct gfx_window *win, 
			 void *font_ptr, char *ch, 
			 float size, float *w, float *h)
{
	win->font_glyph_size(font_ptr, ch, size, w, h);
}



struct gfx_callbacks *gfx_callbacks(struct gfx_window *w)
{
	return w->callbacks(w->impl);
}

static struct gfx_window* instance;
/* Internal get function for win to prevent moving window instance around.
 */
struct gfx_window* gfx_window_instance() { return instance; }
struct gfx_window* gfx_create_window(int x, int y, int w, int h)
{
	struct gfx_window *ret = NULL;

	char *api = getenv("VOB_API");
	if (api == NULL)
		api = "opengl";
	if (strcmp(api, "cairo") == 0) {
#if GFX_API_CAIRO
		ret = gfx_cairo_create_window(x,y,w,h);
#else
		fprintf(stderr, "Cairo API not linked in.");
		exit(1);
#endif	
	}
	if (strcmp(api, "opengl") == 0) {
#if GFX_API_OPENGL
		ret = gfx_opengl_create_window(x,y,w,h);
#else
		fprintf(stderr, "OpenGL API not configured or linked in".);
		exit(1);
#endif
	}


	struct gfx_callbacks *cb = gfx_callbacks(ret);
	cb->generate_scene = &lob_main_generate_vob_scene;
	cb->key_pressed = NULL;
	cb->key_released = NULL;

	ret->anim = gfx_anim_create();
	ret->render = gfx_render_create();

	instance = ret;
	return ret;
}

static char buff[64]; 
void gfx_main_loop(struct gfx_window *win) 
{
	int gui_fd = win->gui_fd(win->impl);
	int gfx_chg_fd = win->anim->pipe_fd[0];
	fd_set fds;
	int val;

	int max = 1 + (gui_fd > gfx_chg_fd? gui_fd: gfx_chg_fd);

	while (1) {
		FD_ZERO(&fds);
		FD_SET(gui_fd, &fds);
		FD_SET(gfx_chg_fd, &fds);

		val = select(max, &fds, NULL, NULL, NULL);
		if (val == -1)
			break;

		// handle gui first
		if (FD_ISSET(gui_fd, &fds)) {
			printf("gui indication\n");
			win->event_handler(win->impl);
		}
		
		// then handle animation changes
		if (FD_ISSET(gfx_chg_fd, &fds)) {
			printf("chg indication\n");
			// make pipe empty..
			read(gfx_chg_fd, buff, sizeof(buff));

			int chg = gfx_anim_chg_reset(win->anim);
			long anim = gfx_anim_time_reset(win->anim);
			printf("chg: %d\n", chg);

			//int chg = 213; // fetch/pop GFX_ANIM_CHG..

			gfx_render(win, chg, anim);
		}
	}

	// close window
	// close pipe
}
