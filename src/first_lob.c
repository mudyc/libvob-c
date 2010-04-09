
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

	l = lob_rect(reg, lob_color(reg, 1.0, 0.5, 0.2));

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

	gfx_main_loop(win);
	return 0;
}
