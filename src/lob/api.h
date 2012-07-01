
#ifndef LOB_API_H
#define LOB_API_H

#include <stdbool.h>
#include <math.h>

#include "vob/scene.h"
#include "util/regions.h"

//#define LOB_INF  (1 << 30)
extern const float LOB_INF;// = 1073741824.0f; // 2^30

struct lob_size { 
	// these values shall be only readable outside of an object
	// (think what happens for glue..)
	float minw, natw, maxw;
	float minh, nath, maxh;
};
typedef struct lob_size Size;

Size *lob_size(Region *r, 
	float minw, float natw, float maxw,
	float minh, float nath, float maxh);


enum EV_TYPE {
	LOB_PTR_PRESS = 1,
	LOB_PTR_RELEASE,
	LOB_PTR_MOVE,
	LOB_KEY
};

struct lob_ev {
	enum EV_TYPE type;
	int x,y;
	bool used;
};
typedef struct lob_ev LobEv;

/**
 * @model
 */
struct lob_model {

};
typedef struct lob_model LobModel;



struct lob;
typedef struct lob Lob;

/**
 * @needs LobModel
 */
struct lob {
	Region *reg;
	Coordsys *cs;

	void (*event)(Lob *l, LobEv *event);

	Size *(*size)(Lob *l);
	Lob * (*layout)(Lob *l, float w, float h);
	void (*render)(Lob *l, Coordsys *into, float w, float h, Scene *vs);
};



void lob_event(Lob *l, LobEv *event);
Size *lob_size_default(Lob *l);
Lob *lob_layout(Lob *this, float w, float h);

//Lob *lob_layout(Lob *l, int w, int h);
void lob_render(Lob *l, Scene *sc);



struct lob_delegate {
	Lob base;
	Lob *delegate;
};
typedef struct lob_delegate LobDelegate;

void lob_delegate_event(Lob *l, LobEv *event);
Size *lob_delegate_size(Lob *l);
Lob *lob_delegate_layout(Lob *l, float w, float h);
void lob_delegate_render(Lob *l, Coordsys *into, float w, float h, Scene *vs);






#endif
