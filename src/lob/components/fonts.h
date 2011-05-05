#ifndef LOB_COMPONENTS_FONTS
#define LOB_COMPONENTS_FONTS

// (c): Matti Katila, 2011


#include "util/array.h"


/** A font API for libvob-c. First just list all fonts, 
 * then render to texture with pango.
 */

struct lob_font {
	char *family;
};
typedef struct lob_font LobFont;

struct lob_glyph {
	LobFont *font;
};
typedef struct lob_glyph LobGlyph;



//LobFont *lob_font(Region *reg, char *family);

//LobGlyph *lob_font_glyph(Region *reg, LobFont *f, char *UTF8characher);

LobHBox *lob_font_text(Region *reg, LobFont *f, float size, char *text);

/** @return Array<LobFont>
 */
UtilArray *lob_font_list(Region *reg);



#endif
