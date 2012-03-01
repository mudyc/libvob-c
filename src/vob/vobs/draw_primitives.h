
#ifndef VOB_VOBS_DRAWING_PRIMITIVES_H
#define VOB_VOBS_DRAWING_PRIMITIVES_H

#include "lob/api.h"
#include "util/array.h"

struct vob_fill {
	char *id;
};
typedef struct vob_fill VobFill;

enum SHADE_TYPE {
	LINEAR = 1,
	CIRCLE = 2
};

struct vob_shade {
	VobFill base;
	enum SHADE_TYPE type;
	float angle; // 0..2PI
	UtilArray *colors; // <VobColor>
	UtilArray *steps; // <float>
};
typedef struct vob_shade VobShade;

struct vob_color {
	VobFill base;
	float r, g, b;
};
typedef struct vob_color VobColor;


VobShade *vob_shade(Region *reg, enum SHADE_TYPE type, float angle);
void vob_shade_add(Region *reg, VobShade *shade, float step, VobColor *c);

VobColor *vob_color(Region *reg, float r, float g, float b);


#endif 
