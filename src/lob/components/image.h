#ifndef LOB_COMPONENTS_IMAGE_H
#define LOB_COMPONENTS_IMAGE_H

#include "lob/api.h"

struct lob_image {
	Lob base;
	char *file;

	Size size;
};
typedef struct lob_image LobImage;
Lob *lob_image(Region *reg, char *file);




#endif
