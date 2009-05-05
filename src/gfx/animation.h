
#ifndef GFX_ANIMATION_H
#define GFX_ANIMATION_H

#include <stdbool.h>

struct gfx_animation {
	int chg;
	int pipe_fd[2];
	long time;
	long time_tmp;
};


enum GFX_ANIM_TYPE {
	/** Changes to next vob scene without animation. Used when
	 * fast menu opens for example. Switch is most powerful
	 * operation, i.e. if there is a pending animation or rerender
	 * it will be replaced with switch instead.
	 */
	CHG_SWITCH_VOB_SCENE,

	/** Shows animation between two vob scenes. The usual way to
	 * change view. Animation is more powerful than rerender
	 * operation, i.e. animation is used instead of rerender if
	 * both are called.
	 */
	CHG_ANIMATE,

	/** Used by background process/thread to update texture
	 * information for instance. Does not change anything on current vob scene.
	 */
	CHG_RERENDER,

};

struct gfx_animation *gfx_anim_create();

void gfx_anim_chg(struct gfx_animation *animation, 
		  enum GFX_ANIM_TYPE type);
int gfx_anim_chg_reset(struct gfx_animation *animation);


void gfx_anim_time(struct gfx_animation *animation,
		   long time_in_ms, bool temporary);
long gfx_anim_time_reset(struct gfx_animation *animation);



#endif
