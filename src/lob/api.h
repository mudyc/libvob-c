
#ifndef LOB_API_H
#define LOB_API_H

#include <math.h>

#include "vob/scene.h"
#include "util/regions.h"

//#define LOB_INF  (1 << 30)
extern const float LOB_INF;// = 1073741824.0f; // 2^30

struct lob_size {
	float minw, natw, maxw;
	float minh, nath, maxh;
};
typedef struct lob_size Size;


struct lob;
typedef struct lob Lob;

struct lob {
	Region *reg;

	Size *(*size)(Lob *l);
	Lob * (*layout)(Lob *l, float w, float h);
	void (*render)(Lob *l, Coordsys *into, float w, float h, Scene *vs);
};


Size *lob_size(Lob *l);


//Lob *lob_layout(Lob *l, int w, int h);
void lob_render(Lob *l, Scene *sc);









#endif
