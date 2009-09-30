
#ifndef GFX_RENDER_H
#define GFX_RENDER_H

#include <time.h>
#include "gfx/animation.h"

struct gfx_render {
	Scene *prev, *current;
	// time stamps for animation
	struct timespec t0, t1;

	void *matcher;
};

struct gfx_render *gfx_render_create();

void gfx_render(struct gfx_window *w, 
		enum GFX_ANIM_TYPE chg,
		long anim_time
		);


#endif
