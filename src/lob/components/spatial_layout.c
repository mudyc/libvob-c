
#include <stdarg.h>


#include "lob/components/spatial_layout.h"


// -------------------------------------------
// Lists - Boxes
// -------------------------------------------

Lob *lob_vbox(Region *reg)
{
	LobVBox *ret = REGION(reg, "lob.component.VBox", LobVBox);
	ret->items = util_arr_create(reg);
	return ret;
}

void lob_vbox_add(Region *reg, LobVBox *vbox, Lob *lob)
{
	util_arr_add(reg, vbox, lob);
}
