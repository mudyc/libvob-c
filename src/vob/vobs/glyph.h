#ifndef VOB_VOBS_GLYPH
#define VOB_VOBS_GLYPH


#include "vob/api.h"
#include "vob/scene.h"
#include "vob/vobs/draw_primitives.h"

struct _vob_glyph {
	Vob base;
	char *family;
	char *ch;
	VobColor *color;
};
typedef struct _vob_glyph Glyph;

Vob1 *vob_glyph(Scene *vs, float size, char *family, char *ch, VobColor *c);




#endif

