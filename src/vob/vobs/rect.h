#ifndef VOB_VOBS_RECT_H
#define VOB_VOBS_RECT_H

#include "vob/api.h"
#include "vob/scene.h"
#include "vob/vobs/draw_primitives.h"

struct vob_rect {
	Vob base;
	float r, g, b;
};
typedef struct vob_rect Rect;

Vob1 *vob_rect(Scene *vs, float r, float g, float b);

struct vob_rounded_rect {
	Vob base;
	VobFill *fill;
	float t,b,l,r;
};
typedef struct vob_rounded_rect VobRoundedRect;

Vob1 *vob_rounded_rect(Scene *vs, VobFill *fill);


#endif
