
#ifndef VOB_SCENE_H
#define VOB_SCENE_H

//! @nopython

#include <glib.h>

#include "util/regions.h"

#include "vob/api.h"
//#include "vob/coords.h"
#include "vob/matcher.h"

/** Vob scene which holds visual objects (vobs) within a map. A vob
 * may be placed according to coordinates (i.e. translations suchs as
 * scale or rotate). If animation is performed between two vob scenes
 * a matcher is used. The matching entity in scene A is animated to
 * matching entity in scene B. Matching is done between given keys and
 * coordinate systems.
 */
struct vob_scene {
	// vob map or list
	// coords
	// matcher
	VobMatcher *matcher;

	FastArray *vobs_arr;
	FastArray *coords_arr;
	//FastArray *anims_arr;
	GHashTable *anim_set;

	Coordsys *rootCS; // coordinates

	Region *reg;

	Scene *previous;

};
//typedef struct vob_scene Scene;

void vob_scene_dump(Scene *vs);

Scene *vob_scene_create(void);
void vob_scene_clear(Scene *sc);

void vob_scene_put0(Scene *vs, Vob0 *vob);
void vob_scene_put1(Scene *vs, Vob1 *vob, Coordsys *cs);
void vob_scene_put2(Scene *vs, Vob2 *vob, Coordsys *cs1, Coordsys *cs2);


#endif
