#ifndef VOB_VOBS_RECT_H
#define VOB_VOBS_RECT_H

#include "vob/api.h"
#include "vob/scene.h"

struct _vob_rect {
	Vob base;
	float r, g, b;
};
typedef struct _vob_rect Rect;

Vob1 *vob_rect(Scene *vs, float r, float g, float b);


#endif
