#include <stdio.h>
#include <stdlib.h>
#include "vob/api.h"
#include "vob/coords.h"
#include "gfx/opengl/coords.h"


void gfx_opengl_coords(Coordsys *cs)
{
	switch (cs->type) {
	case CS_ROOT:
		break;
	case CS_BOX: {// parent , w, h
		BoxCS *p = (BoxCS *)cs;
		gfx_opengl_coords(p->parent);
		break;
	}
	case CS_ORTHO:
	default:
		printf("Un supported coordinate system: %d!\n", cs->type);
		abort();
	}

}
