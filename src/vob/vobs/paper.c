
#include "vob/api.h"
#include "vob/vobs/paper.h"
#include "util/regions.h"




Vob1 *vob_paper(Scene *vs, unsigned long identity)
{
	static char *id = "vob.vobs.Paper";
	Paper *ret = (Paper*) 
		util_regs_instantiate(vs->reg, id, sizeof(Paper));

	ret->base.type = VOB1;
	ret->base.id = id;

	ret->id = identity;

	return (Vob1*)ret;
}
