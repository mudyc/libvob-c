#ifndef LOB_MODELS_KEYANIMMODEL_H
#define LOB_MODELS_KEYANIMMODEL_H

#include <stdbool.h>

#include "lob/api.h"

/**
 * @model
 */
struct lob_animmodel {
	LobModel base;
	float time; // seconds
	float t; // 0 < t < time

	// return true if done - no more ticks
	bool (*tick)(struct lob_animmodel* m, float ms);
};
typedef struct lob_animmodel LobAnimModel;


/**
 * @model
 */
struct lob_keyanimmodel {
	LobAnimModel base2;
	//float time; // seconds

	bool inited;
	float dx, dy, dz, dsx, dsy;
	OrthoCS *cs;

};
typedef struct lob_keyanimmodel LobKeyAnimModel;

LobKeyAnimModel *lob_keyanimmodel(Region *reg, float time);

void lob_keyanimmodel_render(LobKeyAnimModel *m, Lob *that, 
			Coordsys *from, Coordsys *into, 
			float w, float h, Scene *vs); 

#endif
