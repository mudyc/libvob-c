
#include "lob/models/clickmodel.h"


LobClickModel *lob_clickmodel(Region *reg)
{
	LobClickModel *ret = REGION(reg, "lob.model.Click", LobClickModel);
	ret->state = RELEASED;
	ret->clicked = NULL;
	//printf("create click model: %p %d\n", ret, ret->state);
	return ret;
}

bool lob_clickmodel_set(LobClickModel *c, enum CLICK_STATE next)
{
	bool chg = c->state != next;
	c->state = next;
	if (chg && c->state == RELEASED && c->clicked != NULL)
		c->clicked(c);
	return chg;
}
