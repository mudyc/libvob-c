
#ifndef LOB_MODELS_CLICKMODEL_H
#define LOB_MODELS_CLICKMODEL_H

#include <stdbool.h>
#include "lob/api.h"

enum CLICK_STATE {
	PRESSED = 1,
	RELEASED = 2
};


/**
 * @model
 */
struct lob_clickmodel {
	LobModel base;
	enum CLICK_STATE state;
	/** @callback */
        void (*clicked)(LobModel *m);
};
typedef struct lob_clickmodel LobClickModel;

LobClickModel *lob_clickmodel(Region *reg);
bool lob_clickmodel_set(LobClickModel *c, enum CLICK_STATE next);


#endif
