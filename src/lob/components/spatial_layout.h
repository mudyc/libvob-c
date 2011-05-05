
#ifndef LOB_COMPONENTS_SPATIAL_H
#define LOB_COMPONENTS_SPATIAL_H

#include "lob/api.h"
#include "util/array.h"

/** All components that do layouting on spatial (read 2D) space.
 */ 

/*
Lob *lob_left(Lob *l);
Lob *lob_center(Lob *l);
Lob *lob_right(Lob *l);


Lob *lob_glue();
Lob *lob_vglue();
Lob *lob_hglue();
*/


struct lob_vbox {
	Lob base;
	UtilArray *items;
	Size *tmp_size;
};
typedef struct lob_vbox LobVBox;

Lob *lob_vbox(Region *reg);
void lob_vbox_add(Region *reg, LobVBox *vbox, Lob *lob);
//void lob_vlist_addv(Region *reg, LobVList *vlist, int count, Lob **lobs);
//Lob *lob_hlist();

struct lob_hbox {
	Lob base;
	UtilArray *items;
	Size *tmp_size;
};
typedef struct lob_hbox LobHBox;

Lob *lob_hbox(Region *reg);
void lob_hbox_add(Region *reg, LobHBox *hbox, Lob *lob);



#endif
