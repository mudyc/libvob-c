
#include <stdio.h>
#include <stdlib.h>

#include "gfx/gfx_api.h"
#include "vob/scene.h"
#include "lob/api.h"
#include "lob/lobs.h"
#include "util/regions.h"


static Lob* create_lob(Region *reg)
{
	Lob *l;

	printf("create Lob\n");
	l = lob_vbox(reg);
	printf("create Lob\n");
	lob_vbox_add(reg, (LobVBox *)l,
		     lob_rect(reg, lob_color(reg, 1.0, 0.5, 0.2)));
	printf("create Lob\n");
	UtilArray *fonts = lob_font_list(reg);
	printf("create Lob\n");
	lob_vbox_add(reg, (LobVBox *)l, 
		     lob_font_text(reg, util_arr_get(fonts, 0), 20, "Eeti"));
	lob_vbox_add(reg, (LobVBox *)l,
		     lob_rect(reg, lob_color(reg, 1.0, 0.5, 0.2)));

	return l;
}


static void key_pressed()
{
	printf("KEY PRESSED...\n");
}

static void key_released()
{
	printf("KEY RELEASED...\n");
}

int main(int argc, char **argv) 
{
	struct gfx_window *win = gfx_create_window(0, 0, 320, 240);

	struct gfx_callbacks *cb = gfx_callbacks(win);
	cb->create_lob = &create_lob;
	cb->key_pressed = &key_pressed;
	cb->key_released = &key_released;

	g_type_init();
	setup_sigsegv();

	gfx_main_loop(win);
	return 0;
}
