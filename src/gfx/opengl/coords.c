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
	case CS_BOX: {// parent , x,y, w, h
		BoxCS *p = (BoxCS *)cs;
		printf("box: %d\n", p->y);
		gfx_opengl_coords(p->parent);
		if (p->x != 0 || p->y != 0)
			glTranslatef(p->x, p->y, 0);
		break;
	}
	case CS_ORTHO:
	default:
		printf("Un supported coordinate system: %d!\n", cs->type);
		abort();
	}

}
