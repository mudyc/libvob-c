
#include <stdarg.h>


#include "lob/components/spatial_layout.h"


// -------------------------------------------
// Lists
// -------------------------------------------

LobVList *lob_vlist(Region *reg, ...)
{
	LobVList *ret = REGION(reg, "lob.component.VList", LobVList);



	return ret;
}

