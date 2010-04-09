
#ifndef LOB_COMPONENTS_DRAWING_PRIMITIVES_H
#define LOB_COMPONENTS_DRAWING_PRIMITIVES_H

#include "lob/api.h"



struct lob_color {
	float r, g, b;
};
typedef struct lob_color LobColor;

struct lob_rect {
	Lob lob;
	LobColor *color;
};
typedef struct lob_rect LobRect;


LobColor *lob_color(Region *reg, float r, float g, float b);

Lob *lob_rect(Region *reg, LobColor *color);


//lob_color_









#endif 
