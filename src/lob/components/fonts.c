
#include <pango/pango.h>
#include <pango/pangocairo.h>

#include <glib.h>

#include "lob/components/fonts.h"
#include "vob/vobs/glyph.h"

static Size *glyph_size(Lob *l)
{
	LobGlyph *g = (LobGlyph*)l;
	Size *s = g->tmp_size;
	s->minw = s->natw = s->maxw = g->size/2;
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

LobGlyph *lob_glyph(Region *reg, LobFont *font, float size, char *text)
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
		printf("%p %p %p\n", reg, ret, g);
		lob_hbox_add(reg, ret, g);
	}
	return ret;
}


UtilArray *lob_font_list(Region *reg)
{
	printf("lob_font_list\n");
	UtilArray *ret = util_arr_create(reg);
	//PangoContext *context = pango_context_new();
	PangoFontFamily **families;
	int i,count;
	PangoFontMap * fontmap;

	fontmap = pango_cairo_font_map_get_default();
	pango_font_map_list_families (fontmap, &families, &count);
	//pango_context_list_families(context, &families, &count);
	printf("families count %d\n", count);
	for (i=0; i<count; i++) {
		PangoFontFamily *family = families[i];
		const char *name = pango_font_family_get_name(family);
		printf("font name %s\n", name);
		LobFont *font = lob_font(reg, name);
	}
	
	g_free(families);
	//g_object_unref(context);

	return ret;
}
