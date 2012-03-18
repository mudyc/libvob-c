
#include "vob/vobs/glyph.h"


Vob1 *vob_glyph(Scene *vs, float size, char *family, char *ch, VobColor *c)
{
	static char *id = "vob.vobs.Glyph";
	Glyph *ret = (Glyph*) 
		util_regs_instantiate(vs->reg, id, sizeof(Glyph));
	ret->base.type = VOB1;
	ret->base.id = id;

	ret->ch = ch;
	ret->family = family;
	ret->color = c;
	return (Vob1*)ret;
}
