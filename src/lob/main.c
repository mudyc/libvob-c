
#include "gfx/gfx_api.h"
#include "lob/api.h"
#include "vob/scene.h"
#include "util/dbg.h"

static Region *reg = NULL;

// Store lob for events.
static Lob *currentLob = NULL; 

static void debug_objects_recursive(void *o, int indent) {
	int i;
	for (i=0; i<indent; i++) {
		printf("  ");
	}
	//DBG("%s w(%f %f) h(%f %f)", util_regs_dbg(o), );
}
static void debug_objects(void *o) {
	debug_objects_recursive(o, 0);   
}


void lob_main_handle_event(struct gfx_window *win, LobEv* event)
{
	if (reg == NULL )
		return;
	printf("lob.main.event %d %d\n", event->x, event->y);
	currentLob->event(currentLob, event);
}


Scene* lob_main_generate_vob_scene(struct gfx_window *win) {
	util_regs_dbg(NULL);
	// Reserving the scenes in here limits the implementation 
	// to one window. Who cares..

	static Scene 
		*scene0 = NULL,
		*scene1 = NULL,
		*scene2 = NULL;

	if (scene0 == NULL) { // do initialize 
		scene0 = vob_scene_create();
		scene1 = vob_scene_create();
		scene2 = vob_scene_create();

		reg = util_regs_create("lobs");
	}

	// rotate scenes as animation is done between two of them.
	Scene* tmp = scene0;
	scene0 = scene1;
	scene1 = scene2;
	scene2 = tmp;

	vob_scene_clear(scene0);

	scene0->previous = scene1;

	struct gfx_callbacks *cb = gfx_callbacks(win);

	// do lobs
	printf("lob.main.create_lob\n");
	Lob *lob = cb->create_lob(reg);

	printf("lob.main.layout\n");
	printf("%p %d %d\n", lob, gfx_width(win), gfx_height(win));
	lob = lob->layout(lob, gfx_width(win), gfx_height(win));

	debug_objects(lob);

	printf("lob.main.render\n");
	lob->render(lob, scene0->rootCS,
		    gfx_width(win), gfx_height(win), scene0);

	currentLob = lob;

	//util_regs_clear(reg);
/*
	// dump scene info
	int i;
	printf("Scene info: %d\n", scene0->vobs_arr->index);
	for (i=0; i<scene0->vobs_arr->index; i++) {
		Vob *v = util_fastarr_get(scene0->vobs_arr, i);
		
		switch (v->type) {
		case VOB0:
			printf("vob0\n");
			break;
		case VOB1: {
			printf("vob1 %s %p\n", v->id, v);
			Coordsys *cs = util_fastarr_get(scene0->coords_arr, i);
			printf("cs %p\n", cs);
			vob_coords_dump(cs);
			break;
		}
		case VOB2:
		default:
			printf("Vob type not implemented.\n");
			break;
		}
	}
*/


	return scene0;
}


