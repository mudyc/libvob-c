
#include <GL/gl.h>
#include <string.h>
#include <math.h>

#include "gfx/gfx_api.h"
#include "gfx/render.h"
#include "util/dbg.h"

#include "lob/models/keyanimmodel.h"



struct gfx_render *gfx_render_create()
{
	struct gfx_render *ret = malloc(sizeof(struct gfx_render));
	memset(ret, 0, sizeof(struct gfx_render));
	ret->current = NULL;
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
	//DBG("t0  %ld.%ld", render->t0.tv_sec, render->t0.tv_nsec)
	//DBG("now %ld.%ld", now->tv_sec, now->tv_nsec)
	//DBG("t1  %ld.%ld", render->t1.tv_sec, render->t1.tv_nsec)
	long long D = render->t1.tv_sec - render->t0.tv_sec;
	D *= 1000 * 1000 * 1000;
	D += render->t1.tv_nsec - render->t0.tv_nsec;
	
	long long d = now->tv_sec - render->t0.tv_sec;
	d *= 1000 * 1000 * 1000;
	d += now->tv_nsec - render->t0.tv_nsec;
	
	//DBG("1 %lld %lld", d, D);
	float persentage = (double)d/(double)D;
	//DBG("2 %f", persentage);
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
	//DBG("%d",chg)
	struct timespec now;

	struct gfx_callbacks *cb = gfx_callbacks(w);
	struct gfx_render *render = w->render;

	if (!cb->generate_scene)
		return;

	switch (chg) {
	case CHG_SWITCH_VOB_SCENE: {
		render->prev = render->current;
		render->current = cb->generate_scene(w);
		clock_gettime(CLOCK_REALTIME, &render->t0);
		memcpy(&render->t1, &render->t0, sizeof(struct timespec));
		break;
	}
	case CHG_ANIMATE: {
		render->prev = render->current;
		render->current = cb->generate_scene(w);

		clock_gettime(CLOCK_REALTIME, &render->t0);
		memcpy(&render->t1, &render->t0, sizeof(struct timespec));
		render->t1.tv_sec += anim_time / 1000;
		render->t1.tv_nsec += (anim_time % 1000)*1000;
		DBG("t0 %d.%d  t1 %d.%d",render->t0.tv_sec ,render->t0.tv_nsec, render->t1.tv_sec ,render->t1.tv_nsec )
		break;
	}
        case CHG_RERENDER:
	default:
		break;
	}

	// just render in here.

	clock_gettime(CLOCK_REALTIME, &now);
	render->t_since_ms = (now.tv_sec - render->t_frame.tv_sec) * 1000
		+ (now.tv_nsec - render->t_frame.tv_nsec)/1000000.0f;
	render->t_frame.tv_sec = now.tv_sec;
	render->t_frame.tv_nsec = now.tv_nsec;

	if (time_cmp(render->t0, render->t1) < 0 
		&& time_cmp(render->t_frame, render->t1) < 0) {
		//float d = get_anim_fract(render, &now);
		//DBG("fract: %f",render->t_frame)
		gfx_render_frame(w);
		//w->anim_render(w, d);
	} else {
		// no anim...

		GHashTableIter iter;
		gpointer key, value;

		g_hash_table_iter_init (&iter, render->current->anim_set);
		while (g_hash_table_iter_next (&iter, &key, &value)) 
		{
			float ms = render->t_since_ms;
			LobAnimModel *m = (LobAnimModel *)key;
			if (m->tick(m, &now)) {
				g_hash_table_iter_remove(&iter);
				DBG("remove %p",m);
			}
		}



		//printf("no anim\n");
		w->single_render(w, render->current);
	}

}



void gfx_render_frame(struct gfx_window *w)
{
	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);

	struct gfx_render *render = w->render;
	float d = get_anim_fract(render, &now);
	DBG("fract: %f",d);
	w->anim_render(w, render->prev, render->current, d);
}
bool gfx_render_is_animating(struct gfx_window *w)
{
	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);

	struct gfx_render *render = w->render;
	if (time_cmp(render->t0, render->t1) < 0 
		&& time_cmp(now, render->t1) < 0)
		return true;
	return false;
}

bool gfx_render_has_animation_models(struct gfx_window *w)
{
	struct gfx_render *render = w->render;
	if (render->current == NULL)
		return false;
	//DBG("has animmodels? %d", g_hash_table_size(render->current->anim_set))
	return g_hash_table_size(render->current->anim_set) > 0;
}
