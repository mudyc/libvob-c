#ifndef LOB_COMPONENTS_PAPER
#define LOB_COMPONENTS_PAPER

// (c): Matti Katila, 2011

#include "lob/api.h"

struct _lob_paper {
	Lob lob;
	unsigned long id; // hash or something
};
typedef struct _lob_paper LobPaper;


LobPaper *lob_paper(Region *reg, unsigned long id);

#endif
