// (c): Matti J. Katila

#ifndef LOB_MAIN_H
#define LOB_MAIN_H

#include "vob/scene.h"

void lob_main_handle_event(struct gfx_window *win, LobEv* event);
Scene *lob_main_generate_vob_scene(struct gfx_window *w);



#endif
