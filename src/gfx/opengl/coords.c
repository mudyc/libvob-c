#include <stdio.h>
#include <stdlib.h>
#include "vob/api.h"
#include "vob/coords.h"
#include "vob/matcher.h"
#include "gfx/opengl/coords.h"

#include <GL/gl.h> 

void gfx_opengl_coords(Coordsys *cs)
{
	switch (cs->type) {
	case CS_ROOT:
		break;
	case CS_BOX: {// parent , x,y, w, h
		BoxCS *p = (BoxCS *)cs;
		gfx_opengl_coords(p->parent);
		if (p->x != 0 || p->y != 0) {
			glTranslatef(p->x, p->y, 0);
		}
		break;
	}
	case CS_TRANS: {// parent , x,y,z 
		TransCS *p = (TransCS *)cs;
		gfx_opengl_coords(p->parent);
		if (p->x != 0 || p->y != 0 || p->z != 0) {
			//printf("tr: %2.2f %2.2f\n", p->x, p->y);
			glTranslatef(p->x, p->y, p->z);
		}
		break;
	}
	case CS_ORTHO: {// parent , x,y,z , sx,sy
		OrthoCS *p = (OrthoCS *)cs;
		gfx_opengl_coords(p->parent);
		if (p->x != 0 || p->y != 0 || p->z != 0) {
			//printf("tr: %2.2f %2.2f\n", p->x, p->y);
			glTranslatef(p->x, p->y, p->z);
		}
		glScalef(p->sx, p->sy, 1);
		break;

	}
	default:
		printf("Un supported coordinate system: %d!\n", cs->type);
		abort();
	}

}
