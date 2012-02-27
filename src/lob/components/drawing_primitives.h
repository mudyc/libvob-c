
#ifndef LOB_COMPONENTS_DRAWING_PRIMITIVES_H
#define LOB_COMPONENTS_DRAWING_PRIMITIVES_H

#include "lob/api.h"
#include "util/array.h"
#include "vob/vobs.h"
/*

struct lob_fill {

};
typedef struct lob_fill LobFill;

enum SHADE_TYPE {
	LINEAR = 1,
	CIRCLE = 2
};

struct lob_shade {
	LobFill base;
	enum SHADE_TYPE type;
	float angle; // 0..2PI
	UtilArray *colors; // <LobColor>
	UtilArray *steps; // <float>
};
typedef struct lob_shade LobShade;

struct lob_color {
	LobFill base;
	float r, g, b;
};
typedef struct lob_color LobColor;

//LobShade *lob_shade(Region *reg, enum SHADE_TYPE type, float angle);
//LobShade *lob_shade_add(Region *reg, LobShade *shade, float step, VobColor *c);

//LobColor *lob_color(Region *reg, float r, float g, float b);

*/
struct lob_rect {
	Lob lob;
	VobColor *color;
};
typedef struct lob_rect LobRect;


Lob *lob_rect(Region *reg, VobColor *color);

struct lob_rounded_rect {
	Lob lob;
	VobFill *fill;
	float t,b,l,r;
};
typedef struct lob_rounded_rect LobRoundedRect;

Lob *lob_rounded_rect(Region *reg, VobFill *fill, 
		float top, float bottom, float left, float right);

//lob_color_









#endif 
