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
	Lob *(*create_lob)(Region *reg);
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

	void (*font_list)(int *len, char ***list);
	void (*font_glyph_size)(//void *, 
		void *font_ptr, char *ch, float size, 
		float *w, float *h); 

	struct gfx_animation *anim;
	struct gfx_render *render;
};
// namespace problems with Xlib for instance
//typedef struct window indow;



struct gfx_window* gfx_create_window(int x, int y, int w, int h);
struct gfx_window* gfx_window_instance();
void gfx_delete_window(struct gfx_window *win);


struct gfx_callbacks *gfx_callbacks(struct gfx_window *w);
//void gfx_handle_events(struct window *w);

int gfx_height(struct gfx_window *w);
int gfx_width(struct gfx_window *w);

void gfx_main_loop(struct gfx_window *w);


// Fonts
/* @return a string pointer used in glyph pointing.
 * @see gfx_font_glyph_size
 */
void gfx_font_list(struct gfx_window *win,
		   int *length, char ***list);
void gfx_font_glyph_size(struct gfx_window *win, 
			 void *font_ptr, char *ch, float size, float *w, float *h); 


#endif
