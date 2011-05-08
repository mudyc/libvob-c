#ifndef VOB_VOBS_GLYPH
#define VOB_VOBS_GLYPH


#include "vob/api.h"
#include "vob/scene.h"

struct _vob_glyph {
	Vob base;
	char *family;
	char *ch;
};
typedef struct _vob_glyph Glyph;

Vob1 *vob_glyph(Scene *vs, float size, char *family, char *ch);




#endif

