#ifndef LOB_COMPONENTS_FONTS
#define LOB_COMPONENTS_FONTS

// (c): Matti Katila, 2011

#include "lob/components/spatial_layout.h"
#include "vob/vobs/draw_primitives.h"
#include "util/regions.h"
#include "util/array.h"


/** A font API for libvob-c. First just list all fonts, 
 * then render to texture with pango.
 */

struct lob_font {
	const char *family;
};
typedef struct lob_font LobFont;

struct lob_glyph {
	Lob base;
	LobFont *font;
	float size;
	char *ch;
	Size *tmp_size;
	VobColor *color;
	
	struct gfx_window *win;
};
typedef struct lob_glyph LobGlyph;



LobFont *lob_font(Region *reg, char *family);

//LobGlyph *lob_font_glyph(Region *reg, LobFont *f, char *UTF8characher);

LobBox *lob_font_text(Region *reg, LobFont *f, 
		float size, char *text, VobColor *c);

/**
 *  @classmethod
 *  @return UtilArray<LobFont>
 */
UtilArray *lob_font_list(Region *reg);



#endif
