
#ifndef LOB_MODELS_CLICKMODEL_H
#define LOB_MODELS_CLICKMODEL_H

#include "lob/api.h"

/**
 * @model
 */
struct lob_clickmodel {
	LobModel base;
	int state;
};
typedef struct lob_clickmodel LobClickModel;

LobClickModel *lob_clickmodel(Region *reg);


#endif
