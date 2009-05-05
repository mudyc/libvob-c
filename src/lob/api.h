
#ifndef LOB_API_H
#define LOB_API_H

#include "vob/scene.h"
#include "util/regions.h"

#define LOB_INF  (1 << 30)

struct lob {
	Region *reg;
	int (*min_size)();
	int (*pref_size)();
	int (*max_size)();
	struct lob * (*layout)(struct lob *l, int w, int h);
	void (*render)(struct lob *l, Coordsys *into, int w, int h, Scene *vs);
};
typedef struct lob Lob;


//Lob *lob_layout(Lob *l, int w, int h);
void lob_render(Lob *l, Scene *sc);


int lob_min_size();
int lob_pref_size();
int lob_max_size();

Lob *lob_rect(Region *reg, float r, float g, float b);








#endif
