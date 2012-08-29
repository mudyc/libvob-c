
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "gfx/animation.h"

int GFX_ANIM_CHG = CHG_NOTHING;

struct gfx_animation *gfx_anim_create()
{
	struct gfx_animation *ret = 
		malloc(sizeof(struct gfx_animation));
	if (pipe(ret->pipe_fd) == -1) {
		fprintf(stderr, "Pipe creation failed");
		exit(1);
	}
	ret->time = 2 * 1000;
	return ret; 
}

void gfx_anim_chg(struct gfx_animation *a, enum GFX_ANIM_TYPE type)
{
	switch (type) {
	case CHG_SWITCH_VOB_SCENE:
		a->chg = CHG_SWITCH_VOB_SCENE;
		break;
	case CHG_ANIMATE:
		if (GFX_ANIM_CHG != CHG_SWITCH_VOB_SCENE)
			a->chg = CHG_ANIMATE;
		break;
	case CHG_RERENDER:
	default:
		if (GFX_ANIM_CHG != CHG_SWITCH_VOB_SCENE 
		    && GFX_ANIM_CHG != CHG_ANIMATE)
			a->chg = CHG_RERENDER;
	}
	char buff[1] = { '*' };
	write(a->pipe_fd[1], buff, sizeof(buff));
	
}
int gfx_anim_chg_reset(struct gfx_animation *a)
{
	int ret = a->chg;
	a->chg = CHG_NOTHING;
	return ret;
}
bool gfx_anim_is_chg_incoming(struct gfx_animation *a)
{
	return a->chg != CHG_NOTHING && a->chg != CHG_RERENDER;
}

long gfx_anim_time_reset(struct gfx_animation *a)
{
	long ret = a->time;
	if (a->time_tmp > 0)
		ret = a->time_tmp;
	a->time_tmp = CHG_NOTHING;
	return ret;
}


void gfx_anim_time(struct gfx_animation *a, long time_in_ms, bool temporary)
{

}
