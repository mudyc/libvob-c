
#include <pnglite.h>
#include <GL/gl.h> 

#include "gfx/opengl/image.h"
#include "gfx/opengl/util.h"
#include "vob/coords.h"

struct img {
	png_t png;
	GLuint tex;
	float tw,th;
};

static GHashTable *file2img = NULL;

static struct img *load_image(const char *f) 
{
	printf("load image.. %s\n",f);
	png_init(0, 0);
	struct img *ret = malloc(sizeof(struct img));
	int err = png_open_file(&ret->png, f);
	if (err != PNG_NO_ERROR)
		printf("error in opening image file: %s, %s\n", f, 
			png_error_string(err));

	char data[ret->png.width * ret->png.height * ret->png.bpp];
	png_get_data(&ret->png, &data);

	glGenTextures(1, &ret->tex);
	glBindTexture(GL_TEXTURE_2D, ret->tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
		ret->png.width, ret->png.height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, &data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	png_close_file(&ret->png);

	return ret;
}


void gfx_opengl_image_size(char *file, int *w, int *h)
{
	if(file2img == NULL)
		file2img = g_hash_table_new(g_str_hash, g_str_equal);
	gpointer img = g_hash_table_lookup(file2img, file);
	if (img == NULL)
		g_hash_table_insert(file2img, file, 
				img = load_image(file));
	struct img *i = (struct img *)img;
	*w = i->png.width;
	*h = i->png.height;
	//printf("opengl img size %d %d\n", *w, *h);
}
void gfx_opengl_image(Image *img, Coordsys *cs)
{
	const char *file = img->file;

	if(file2img == NULL)
		file2img = g_hash_table_new(g_str_hash, g_str_equal);
	gpointer pimg = g_hash_table_lookup(file2img, file);
	if (pimg == NULL)
		g_hash_table_insert(file2img, file, 
				pimg = load_image(file));
	struct img *i = (struct img *)pimg;

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_ALWAYS, 0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glError();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, i->tex);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glError();

	float 
		w = vob_coords_w(cs),
		h = vob_coords_h(cs);
	//printf("img: %fx%f %d %d\n", w,h,i->png.width, i->png.height);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex2f(0, h);
	glTexCoord2f(1, 1);
	glVertex2f(w, h);
	glTexCoord2f(1, 0);
	glVertex2f(w, 0);
	glTexCoord2f(0, 0);
	glVertex2f(0, 0);
	glEnd();
}
