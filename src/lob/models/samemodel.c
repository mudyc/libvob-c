
#include "lob/models/samemodel.h"

void lob_samemodel_tick(LobSameModel *m, float value)
{
	m->summ += value;
	m->count++;
}

float lob_samemodel_get(LobSameModel *m)
{
	return m->summ / (float) m->count;
}

LobSameModel *lob_samemodel(Region *reg)
{
	LobSameModel *ret = REGION(reg, "lob.model.Same", LobSameModel);
	ret->summ = 0;
	ret->count = 0;
	return ret;
}
