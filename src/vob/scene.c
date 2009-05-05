
#include <stdlib.h>

#include "vob/scene.h"
#include "vob/coords.h"
#include "util/dbg.h"
#include "util/regions.h"



Scene *vob_scene_create()
{
	Scene *ret = malloc(sizeof(Scene));

	ret->reg = util_regs_create("scene");

	ret->rootCS = (Coordsys*) vob_coords_root(ret);
	ret->vobs_arr = util_fastarr_create(sizeof(void *));
	ret->coords_arr = util_fastarr_create(sizeof(void *));

	return ret;
}

void vob_scene_clear(Scene *sc) 
{
	util_regs_clear(sc->reg);

	util_fastarr_clear(sc->vobs_arr);
	util_fastarr_clear(sc->coords_arr);
}

void vob_scene_put0(Scene *vs, Vob0 *v)
{
	util_fastarr_add(vs->vobs_arr, v);
	util_fastarr_add(vs->coords_arr, NULL);
}
void vob_scene_put1(Scene *vs, Vob1 *v, Coordsys *cs)
{
	util_fastarr_add(vs->vobs_arr, v);
	util_fastarr_add(vs->coords_arr, cs);
}

struct cs_pair {
	Coordsys *cs1, *cs2;
};

void vob_scene_put2(Scene *vs, Vob2 *v, Coordsys *cs1, Coordsys *cs2)
{
	REGION_ID(id, "vob.scene.CoordsysPair");
	struct cs_pair *pair = (struct cs_pair*)
		util_regs_instantiate(vs->reg, &id, sizeof(struct cs_pair));
	pair->cs1 = cs1;
	pair->cs2 = cs2;

	util_fastarr_add(vs->vobs_arr, v);
	util_fastarr_add(vs->coords_arr, pair);
}

