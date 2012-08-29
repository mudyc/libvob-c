
#ifndef GFX_RENDER_H
#define GFX_RENDER_H

#include <time.h>
#include <stdbool.h>
#include "gfx/animation.h"

struct gfx_render {
	Scene *prev, *current;
	// time stamps for animation
	struct timespec t0, t1;

	// current frame timestamp
	struct timespec t_frame;
	// time in ms to previous frame
	long t_since_ms;

};

struct gfx_render *gfx_render_create();

void gfx_render(struct gfx_window *w, 
		enum GFX_ANIM_TYPE chg,
		long anim_time
		);

void gfx_render_frame(struct gfx_window *w);

bool gfx_render_is_animating(struct gfx_window *w);
bool gfx_render_has_animation_models(struct gfx_window *w);

#endif
