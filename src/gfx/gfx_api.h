// (c) Matti J. Katila

#ifndef H_GFX_API
#define H_GFX_API

#include "lob/api.h"
#include "vob/scene.h"


/** Grafix API layer stands for delegating system between the
 * grafix implementation and lob/vob system.
 */

struct gfx_window;

struct gfx_callbacks {
	Scene *(*generate_scene)(struct gfx_window *);
	Lob *(*create_lob)(Region *lobs_reg);
	void (*key_pressed)(void);
	void (*key_released)(void);
};

struct gfx_window {
	void *impl; // pointer to implementation.

	/* All below this are internal function pointers implemented
	 * by the real gfx, e.g. cairo or open gl.
	 */
	void (*event_handler)(void *);
	struct gfx_callbacks *(*callbacks)(void *);
	int (*width)(void *);
	int (*height)(void *);
	int (*gui_fd)(void *);

	void (*single_render)(struct gfx_window *w, Scene *vs);
	void (*anim_render)(Scene *vs, float fract);

	struct gfx_animation *anim;
	struct gfx_render *render;
};
// namespace problems with Xlib for instance
//typedef struct window indow;



struct gfx_window* gfx_create_window(int x, int y, int w, int h);
void gfx_delete_window(struct gfx_window *win);

struct gfx_callbacks *gfx_callbacks(struct gfx_window *w);
//void gfx_handle_events(struct window *w);

int gfx_height(struct gfx_window *w);
int gfx_width(struct gfx_window *w);

void gfx_main_loop(struct gfx_window *w);


#endif
