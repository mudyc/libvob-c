
#include "lob/models/clickmodel.h"


LobClickModel *lob_clickmodel(Region *reg)
{
	LobClickModel *ret = REGION(reg, "lob.model.Click", LobClickModel);
	return ret;
}
