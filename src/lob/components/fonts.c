
#include <pango/pango.h>
#include <pango/pangocairo.h>

#include <glib.h>

#include "gfx/gfx_api.h"
#include "lob/components/fonts.h"
#include "vob/coords.h"
#include "vob/vobs/glyph.h"



static Size *glyph_size(Lob *l)
{
	LobGlyph *g = (LobGlyph*)l;

	float w,h;
	gfx_font_glyph_size(gfx_window_instance(), 
			    g->font->family, g->ch, g->size, &w, &h);
	//printf("wh: %f %f\n", w, h);
	Size *s = g->tmp_size;
	s->minw = s->natw = s->maxw = w;//g->size/2;
	s->minh = s->nath = s->maxh = g->size;
	return s;
}

static Lob *glyph_layout(Lob *l, float w, float h)
{
	return l;
}

static void glyph_render(Lob *l, Coordsys *into, float w, float h, Scene *vs)
{
	LobGlyph *g = (LobGlyph*)l;
	Glyph *vob = vob_glyph(vs, g->size, g->font->family, g->ch);

	// create a sufficient transformation for the vob
	Coordsys *cs = vob_coords_box(vs, into, 0,0,w, h);

	// bind vob to coordinate system
	vob_scene_put1(vs, vob, cs);
}

static LobGlyph *lob_glyph(Region *reg, LobFont *font, float size, char *text)
{
	LobGlyph *ret = REGION(reg, "lob.Glyph", LobGlyph);
	ret->base.size = glyph_size;
	ret->base.layout = glyph_layout;
	ret->base.render = glyph_render;

	ret->tmp_size = REGION(reg, "lob.Size", Size);

	ret->font = font;
	ret->size = size;
	ret->ch = text;
	return ret;
}


LobFont *lob_font(Region *reg, char *family)
{
	LobFont *ret = REGION(reg, "lob.Font", LobFont);
	ret->family = family;
	return ret;
}


LobHBox *lob_font_text(Region *reg, LobFont *f, float size, char *text)
{
	LobHBox *ret = lob_hbox(reg);
	int i;
	for (i=0; text[i]; i++) {
		LobGlyph *g = lob_glyph(reg, f, size, &text[i]);
		lob_hbox_add(reg, ret, g);
	}
	return ret;
}


UtilArray *lob_font_list(Region *reg)
{
	printf("lob_font_list\n");
	UtilArray *ret = util_arr_create(reg);

	int i, len = 0;
	char **fonts;
	gfx_font_list(gfx_window_instance(), &len, &fonts);
	for (i=0; i<len; i++) {
		//printf("font: %s\n", fonts[i]);
		LobFont *font = lob_font(reg, fonts[i]);
		util_arr_add(reg, ret, font);
	}

	return ret;
}
