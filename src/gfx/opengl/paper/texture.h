#ifndef GFX_OPENGL_PAPER_TEXTURE_H
#define GFX_OPENGL_PAPER_TEXTURE_H


#include <GL/gl.h>
#include <stdbool.h>


enum TextureName {
	FNOISE,
	GEOMETRIC
};

struct texture {
	char *name;
	int w, h;
	GLint texture_components;
	GLenum texture_format;
	enum TextureName text_name;
	float g_type, g_scale, g_freq, g_df, g_bias, g_turb, 
		g_freq2, g_seed, g_aniso, g_fbm;
	float scale, featurescale;
	bool continuous;
	GLint minfilter, magfilter;

	GLuint tex;
};
int gfx_opengl_paper_texture_count();
GLuint gfx_opengl_paper_texture_get(int i);
float gfx_opengl_paper_texture_scale_get(int i);
void gfx_opengl_paper_create_textures();
void gfx_opengl_generate_texture_fnoise(struct texture *t, float *data);
void gfx_opengl_generate_texture_geometric(struct texture *t, float *data);

#endif
