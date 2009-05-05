
#ifndef VOB_SCENE_H
#define VOB_SCENE_H

#include "util/regions.h"

#include "vob/api.h"
//#include "vob/coords.h"

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
	FastArray *vobs_arr;
	FastArray *coords_arr;

	Coordsys *rootCS; // coordinates

	Region *reg;
};
//typedef struct vob_scene Scene;

Scene *vob_scene_create();
void vob_scene_clear(Scene *sc);

void vob_scene_put0(Scene *vs, Vob0 *vob);
void vob_scene_put1(Scene *vs, Vob1 *vob, Coordsys *cs);
void vob_scene_put2(Scene *vs, Vob2 *vob, Coordsys *cs1, Coordsys *cs2);


#endif
