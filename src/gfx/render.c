
#include <GL/gl.h>
#include <string.h>
#include <math.h>

#include "gfx/gfx_api.h"
#include "gfx/render.h"
#include "util/dbg.h"




struct gfx_render *gfx_render_create()
{
	struct gfx_render *ret = malloc(sizeof(struct gfx_render));
	memset(ret, 0, sizeof(struct gfx_render));
	return ret;
}

static int time_cmp(struct timespec a, struct timespec b)
{
	time_t d = a.tv_sec - b.tv_sec;
	if (d != 0)
		return d>0?1:-1;
	long nd = a.tv_nsec - b.tv_nsec;
	if (nd == 0)
		return 0;
	else
		return nd>0?1:-1;
}

static float get_anim_fract(struct gfx_render *render, struct timespec *now)
{
	long D = render->t1.tv_sec - render->t0.tv_sec;
	D *= 1000 * 1000;
	D += render->t1.tv_nsec - render->t0.tv_nsec;
	
	long d = now->tv_sec - render->t0.tv_sec;
	d *= 1000 * 1000;
	d += now->tv_nsec - render->t0.tv_nsec;
	
	DBG("1");
	float persentage = d/D;
	DBG("2");
	// gnuplot: 
	//  set xrange[0:2]
	//  plot (1+sin((x*3.7-pi/2)))*0.55

	// in the original implementation there were "waving" at the
	// end of the animation.
	float ret = persentage * 3.7;
	ret -= M_PI/2;
	ret = 1 + sin(ret);
	ret *= 0.55f;
	return ret;
}

void gfx_render(struct gfx_window *w, 
		enum GFX_ANIM_TYPE chg, 
		long anim_time)
{
	struct timespec now;

	struct gfx_callbacks *cb = gfx_callbacks(w);
	struct gfx_render *render = w->render;

	if (!cb->generate_scene)
		return;

	switch (chg) {
	case CHG_SWITCH_VOB_SCENE: {
		render->prev = NULL;
		render->current = cb->generate_scene(w);
		clock_gettime(CLOCK_REALTIME, &render->t0);
		memcpy(&render->t1, &render->t0, sizeof(struct timespec));
		render->matcher = NULL;
		break;
	}
	case CHG_ANIMATE: {
		render->prev = render->current;
		render->current = cb->generate_scene(w);

		// XXX todo matcher..
		render->matcher = NULL;

		clock_gettime(CLOCK_REALTIME, &render->t0);
		memcpy(&render->t1, &render->t0, sizeof(struct timespec));
		render->t1.tv_sec += anim_time / 1000;
		render->t1.tv_nsec += (anim_time % 1000)*1000;
		break;
	}
        case CHG_RERENDER: break;
	}

	// just render in here.

	clock_gettime(CLOCK_REALTIME, &now);

	if (time_cmp(render->t0, render->t1) < 0 
	    && time_cmp(render->t1, now) < 0) {
		/*float d = */get_anim_fract(render, &now);
		//w->anim_render(w, d);
	} else {
		// no anim...
		printf("no anim\n");
		w->single_render(w, render->current);
	}

}
