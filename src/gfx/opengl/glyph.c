// (c) Matti Katila, 2011

#include <GL/gl.h>
#include <stdbool.h>
#include <stdlib.h>

#include <glib.h>

#include "vob/vobs/glyph.h"
#include "gfx/opengl/glyph.h"

static int ch_len(const char *ch) 
{
	int ret = 1;
	unsigned char c = (unsigned char) ch[0];
	if ((c & 0x80) > 0) {
                // 110xxxxx 10xxxxxx - http://en.wikipedia.org/wiki/UTF-8
		if ((c & 0xE0) == 0xC0)  
			ret = 2;
                // 1110xxxx 10xxxxxx 10xxxxxx
		else if ((c & 0xF0) == 0xE0) 
			ret = 3;
		// 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		else if ((c & 0xF8) == 0xF0)
			ret = 4;
		// 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		else if ((c & 0xFC) == 0xF8) 
			ret = 5;
		// 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		else if ((c & 0xFE) == 0xFC)
			ret = 6;
	}
	return ret;
}

static guint ch_hash(gconstpointer key)
{
        // http://developer.gnome.org/glib/2.28/glib-Hash-Tables.html#g-str-hash
	guint hash = 5381; 
	const char *c = key;
	int len = ch_len(c);
	int i;
	for (i=0; i<len; i++)
		hash = hash * 33 + c[i];
	return hash;
}

static gboolean ch_equal(gconstpointer a, gconstpointer b)
{
	const char *ca = a;
	const char *cb = b;
	int la = ch_len(ca);
	int lb = ch_len(cb);
	if (la != lb) 
		return false;
	int i;
	for (i=0; i<la; i++)
		if (ca[i] != cb[i])
			return false;
	return true;
}

struct glyph_tex_coords {
	float tx,ty,tw,th;
};
typedef struct glyph_tex_coords GTexCoords;

struct glyph_tex {
	float x,y,w,h;
	GLuint texture;
	GHashTable *ch2coords;
};
typedef struct glyph_tex GlyphTex;

static GHashTable *family2gtex = NULL;



static GlyphTex *get_texture(char *family, char *ch) {
	if (family2gtex == NULL)
		family2gtex = g_hash_table_new(&g_str_hash, &g_str_equal);

	GlyphTex *gtex = g_hash_table_lookup(family2gtex, family);
	if (gtex == NULL) {
		g_hash_table_insert(family2gtex, family, 
				    gtex = malloc(sizeof(GlyphTex)));
		gtex->ch2coords = 
			g_hash_table_new(&ch_hash, &ch_equal);
	}
	GTexCoords *gcoords = g_hash_table_lookup(gtex->ch2coords, ch);
	if (gcoords == NULL) {
		// possibly increase texture size
		// render glyph(s)
		g_hash_table_insert(gtex->ch2coords, ch, 
				    gcoords = malloc(sizeof(GTexCoords)));
		// XXX
	}
	return NULL;
}



void gfx_opengl_glyph(Vob1 *v, Coordsys *cs)
{
	printf("gfx_opengl_glyph\n");
	Glyph *g = (Glyph*)v;
	

	// get texture for the glyph
	GlyphTex *gt = get_texture(g->family, g->ch);
}
