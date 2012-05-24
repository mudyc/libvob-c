#ifndef LOB_COMPOSITES_BUTTON_H
#define LOB_COMPOSITES_BUTTON_H

#include "lob/api.h"
#include "lob/models/clickmodel.h"

struct lob_button {
	Lob base;
	Lob *delegate;
	LobClickModel *model;
};
typedef struct lob_button LobButton;

Lob *lob_button(Region *reg, char *text, LobClickModel *model);  


#endif
