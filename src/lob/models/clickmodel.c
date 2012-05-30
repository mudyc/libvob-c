
#include "lob/models/clickmodel.h"


LobClickModel *lob_clickmodel(Region *reg)
{
	LobClickModel *ret = REGION(reg, "lob.model.Click", LobClickModel);
	ret->state = RELEASED;
	//printf("create click model: %p %d\n", ret, ret->state);
	return ret;
}
