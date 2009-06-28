
#ifndef LOB_API_H
#define LOB_API_H

#include <math.h>

#include "vob/scene.h"
#include "util/regions.h"

//#define LOB_INF  (1 << 30)
float LOB_INF;

struct size {
	float minw, natw, maxw;
	float minh, nath, maxh;
};
typedef struct size Size;


struct lob;
typedef struct lob Lob;

struct lob {
	Region *reg;

	Size *(*size)(Lob *l);
	Lob * (*layout)(Lob *l, int w, int h);
	void (*render)(Lob *l, Coordsys *into, int w, int h, Scene *vs);
};


Size *lob_size(Lob *l);


//Lob *lob_layout(Lob *l, int w, int h);
void lob_render(Lob *l, Scene *sc);









#endif
