// (c) Matti Katila, 2011

#include <GL/gl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include <glib.h>

#include <fontconfig/fontconfig.h>
#include <ft2build.h>
#include FT_FREETYPE_H
//#include <freetype/freetype.h>

#include "vob/coords.h"
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
	GLuint tex;
	float tx1, ty1;
	float adv,coord_bottom,bearingX,bearingY,w,h;
};
typedef struct glyph_tex_coords GTexCoords;

struct glyphs {
	FT_Face face;
	GHashTable *ch2coords;
};
typedef struct glyphs Glyphs;

static char **font_list = NULL;
static GHashTable *font2file = NULL;
static GHashTable *font2glyphs = NULL;



static GTexCoords *get_texture(char *family, char *ch) {
	Glyphs *glyphs = g_hash_table_lookup(font2glyphs, family);
	GTexCoords *gcoords = g_hash_table_lookup(glyphs->ch2coords, ch);
	if (gcoords->tex == ~0) {
		glGenTextures(1, &gcoords->tex);

		FT_Face face = glyphs->face;
		FT_UInt idx = FT_Get_Char_Index(face, (FT_ULong) *ch);
		FT_Set_Pixel_Sizes(face, 0, 32);
		FT_Vector vec = {1,1};
		//FT_Set_Transform(face, 0, &vec);
		FT_Load_Glyph(face, idx, FT_LOAD_RENDER);
		FT_Bitmap map = face->glyph->bitmap;
		GLsizei  width = 1, height = 1;
		while (map.width > width) width <<= 1;
		while (map.rows > height) height <<= 1;
		
		gcoords->tx1 = (float)map.width / (float) width;
		gcoords->ty1 = (float)map.rows / (float) height;

		GLubyte data[width * height];
		int x,y;
		for (y=0; y<height; y++)
			for (x=0; x<width; x++) {
				if (x >= map.width || y >= map.rows)
					data[x+y*width] = 0;
				else
					data[x+y*width] = map.buffer[x+y*map.pitch];
			}
		
		//memcpy(map.buffer, data, map->width*map->rows);
		glBindTexture(GL_TEXTURE_2D, gcoords->tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
			     GL_LUMINANCE, GL_UNSIGNED_BYTE, data );
	}
	return gcoords;
}



void gfx_opengl_glyph(Vob1 *v, Coordsys *cs)
{

	//printf("gfx_opengl_glyph\n");
	Glyph *g = (Glyph*)v;
	

	// get texture for the glyph
	GTexCoords *gcoords = get_texture(g->family, g->ch);

	float 
		w = vob_coords_w(cs),
		h = vob_coords_h(cs);

	/* Let's assume that height is the thing we want to think as
	 * good one.
	 */
	float 
		x0 = gcoords->bearingX * h,
		x1 = (gcoords->bearingX + gcoords->w)*h,
		y0 = h - (gcoords->bearingY*h),
		y1 = h - (gcoords->coord_bottom*h);

	//printf("%c wh %2.2fx%2.2f\n", *(g->ch),w,h);
	//printf("%c %f %f %f %f\n", *(g->ch), x0, x1, y0, y1);

	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_);
	glEnable(GL_TEXTURE_2D);
	glAlphaFunc(GL_GREATER, 0.15f);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);
	glEnable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, gcoords->tex);

	glBegin(GL_QUADS);
	glTexCoord2f(0,gcoords->ty1);
	glVertex2f(x0, y1);
	glTexCoord2f(gcoords->tx1, gcoords->ty1);
	glVertex2f(x1, y1);
	glTexCoord2f(gcoords->tx1,0);
	glVertex2f(x1, y0);
	glTexCoord2f(0,0);
	glVertex2f(x0, y0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}


void gfx_opengl_font_list(int *length, char ***list)
{
	printf("gfx opengl font list..\n");

	if (font2file != NULL) {
		*length = g_hash_table_size(font2glyphs);
		*list = font_list;
		return;
	}
	font2file = g_hash_table_new(&g_str_hash, &g_str_equal);
	//  g_free, g_free);
	font2glyphs = g_hash_table_new(&g_str_hash, &g_str_equal);

	if (!FcInit()) return;

	int i;
	FcPattern *pat = FcPatternCreate();
	FcObjectSet *os = FcObjectSetBuild(FC_FAMILY, FC_STYLE, FC_FILE, (char *) 0);
	FcFontSet *fs = FcFontList(0, pat, os);
	if (os)
		FcObjectSetDestroy (os);
	if (pat)
		FcPatternDestroy (pat);

	*length = fs->nfont;
	font_list = malloc(sizeof(char*)*fs->nfont);
	*list = font_list;
	//printf("fonts: %d\n", fs->nfont);
	for (i=0; i<fs->nfont; i++) {
		FcPattern *pat = fs->fonts[i];
		font_list[i] = (char*)FcPatternFormat(pat, (FcChar8*)
					       "%{family[0]} %{style}");
		FcChar8 *file = NULL;
		if (FcPatternGetString(pat, FC_FILE, 0, &file) == FcResultTypeMismatch)
			errx(1, "Font congig file get failed (%s)", 
			     font_list[i]);

		g_hash_table_insert(font2file, font_list[i], file);
		g_hash_table_insert(font2glyphs, font_list[i], NULL);
	}
	if (fs)
		FcFontSetDestroy (fs);
	// if this is executed all seems to fall down.
	//FcFini();
}

static Glyphs * create_glyphs(void *font_ptr) {
	Glyphs *ret = malloc(sizeof(Glyphs));
	char *file = g_hash_table_lookup(font2file, font_ptr);

	FT_Library library;
	if (FT_Init_FreeType(&library)) 
		errx(1, "FATAL: FT library init failed");
	FT_Face face;
	if (FT_New_Face(library, file, 0, &face ))
		errx(1, "FATAL: New FT face(%p) failed for font (%s).", 
		     file, font_ptr);
	ret->face = face;
	ret->ch2coords = g_hash_table_new(&ch_hash, &ch_equal);
	return ret;
}

void gfx_opengl_font_glyph_size(void *font_ptr, char *ch, 
				float size, float *w, float *h)
{
	 Glyphs *glyphs = g_hash_table_lookup(font2glyphs, font_ptr);
	 if (glyphs == NULL) {
		 glyphs = create_glyphs(font_ptr);
		 g_hash_table_insert(font2glyphs, font_ptr, glyphs);
	 }

	 GTexCoords *coords = g_hash_table_lookup(glyphs->ch2coords, ch);
	 if (coords == NULL) {
		 g_hash_table_insert(glyphs->ch2coords, ch,
				     coords = malloc(sizeof(GTexCoords)));
		 FT_Face face = glyphs->face;
		 FT_UInt idx = FT_Get_Char_Index(face, (FT_ULong) *ch);
		 FT_Set_Pixel_Sizes(face, 0, 64);
		 FT_Load_Glyph(face, idx, FT_LOAD_DEFAULT);
		 FT_Glyph_Metrics m = face->glyph->metrics;
		 coords->coord_bottom = 
			 (float)(m.horiBearingY - m.height) / 26.6f /64.0f;
		 coords->bearingX = (float)m.horiBearingX / 26.6f / 64.0f;
		 coords->bearingY = (float)m.horiBearingY / 26.6f / 64.0f;
		 coords->w = (float) m.width / 26.6f / 64.0f;
		 coords->h = (float) m.height / 26.6f / 64.0f;
		 coords->adv = (float) m.horiAdvance / 26.6f / 64.0f;
		 coords->tex = ~0;
	 }

	 *w = coords->adv * size;
	 *h = coords->h * size;
}
