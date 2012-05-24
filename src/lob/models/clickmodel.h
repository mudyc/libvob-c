
#ifndef LOB_MODELS_CLICKMODEL_H
#define LOB_MODELS_CLICKMODEL_H

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
};
typedef struct lob_clickmodel LobClickModel;

LobClickModel *lob_clickmodel(Region *reg);


#endif
