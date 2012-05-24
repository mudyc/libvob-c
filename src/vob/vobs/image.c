#include <stdio.h>

#include "vob/api.h"
#include "vob/vobs/image.h"
#include "util/regions.h"




Vob1 *vob_image(Scene *vs, char *file)
{
	static char *id = "vob.vobs.Image";
	Image *ret = (Image*) 
		util_regs_instantiate(vs->reg, id, sizeof(Image));

	ret->base.type = VOB1;
	ret->base.id = id;

	ret->file = file;
	return (Vob1*)ret;
}

