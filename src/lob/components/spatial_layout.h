
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
*/
struct _lob_glue {
	Lob base;
	Size size;
};
typedef struct _lob_glue LobGlue;
Lob *lob_vglue();
Lob *lob_hglue();



struct _lob_box {
	Lob base;
	UtilArray *items;
	Size *tmp_size;
};
typedef struct _lob_box LobBox;

Lob *lob_vbox(Region *reg);
void lob_vbox_add(Region *reg, LobBox *vbox, Lob *lob);
//void lob_vlist_addv(Region *reg, LobVList *vlist, int count, Lob **lobs);
//Lob *lob_hlist();

Lob *lob_hbox(Region *reg);
void lob_hbox_add(Region *reg, LobBox *hbox, Lob *lob);



#endif
