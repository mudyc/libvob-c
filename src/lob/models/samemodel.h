
#ifndef LOB_MODELS_SAMEMODEL_H
#define LOB_MODELS_SAMEMODEL_H

#include "lob/api.h"


/**
 * @model
 */
struct lob_samemodel {
	LobModel base;
	float summ;
	float count;
};
typedef struct lob_samemodel LobSameModel;

void lob_samemodel_tick(LobSameModel *m, float value);
LobSameModel *lob_samemodel(Region *reg);

#endif
