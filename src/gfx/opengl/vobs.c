#include <string.h>
#include <stdio.h>
#include "gfx/opengl/coords.h"
#include "gfx/opengl/vobs.h"
#include "gfx/opengl/glyph.h"
#include "gfx/opengl/paper/paper.h"
#include "vob/coords.h"
#include "vob/vobs/rect.h"
#include "util/primitives.h"
#include <GL/gl.h> 


static GHashTable *shade2tex = NULL;
static guint shade_hash(gconstpointer key)
{
	guint ret = 0x478325D;
	int i;
	VobShade *shade = (VobShade *)key;
	for (i=0; i<shade->steps->size; i++){
		UtilF *f = util_arr_get(shade->steps, i);
		guint bits;
		memcpy(&bits, &f->value, sizeof(float));
		ret = (ret << 13) ^ bits;
	}
	for (i=0; i<shade->colors->size; i++){
		VobColor *col = util_arr_get(shade->colors, i);
		guint bits;
		memcpy(&bits, &col->r, sizeof(float));
		ret = (ret << 13) ^ bits;
		memcpy(&bits, &col->g, sizeof(float));
		ret = (ret << 13) ^ bits;
		memcpy(&bits, &col->b, sizeof(float));
		ret = (ret << 13) ^ bits;
	}
	return ret;
}

static int SHADE_SLIDE = 128;


/**  i   step
 * --+----+---->
 */
static void calculate_shade(int i, int step,float *rgba, float *delta, 
			VobShade *shade)
{
	// count delta
	UtilF *f = util_arr_get(shade->steps, step);
	int j =  f->value * SHADE_SLIDE;
	int d = j - i;
	printf("i %d, j %d, d %d, step %d\n", i, j, d, step);
	VobColor *c = util_arr_get(shade->colors, step);
	VobColor *prev = c;
	if (step - 1 >= 0)
		prev = util_arr_get(shade->colors, step - 1);
	rgba[0] = prev->r;
	rgba[1] = prev->g;
	rgba[2] = prev->b;
	rgba[3] = 1;

	if (d > 0.001){
		delta[0] = (c ->r - rgba[0]) / d;
		delta[1] = (c ->g - rgba[1]) / d;
		delta[2] = (c ->b - rgba[2]) / d;
		delta[3] = (1 - rgba[3]) / d;
	} else {
		delta[0] = delta[1] = delta[2] = delta[3] = 0;
	}
}
static GLuint tex4shade(VobShade *shade) 
{
	if (shade2tex == NULL) {
		shade2tex = g_hash_table_new(g_direct_hash, g_direct_equal);
	}

	guint hash = shade_hash(shade);
	printf("shade hash: %d\n",hash);
	gpointer tex = g_hash_table_lookup(shade2tex, GUINT_TO_POINTER(hash));
	if (tex == NULL) {
		GLuint t;
		float data[SHADE_SLIDE*2*4];
		glGenTextures(1, &t);
		glBindTexture(GL_TEXTURE_2D, t);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		float rgba[4], drgba[4];
		int i=0, j, step=0;
		calculate_shade(i,step, rgba, drgba, shade);
		for (i=0; i<SHADE_SLIDE; i++) {
			memcpy(&data[i*8], rgba, sizeof(float)*4);	
			memcpy(&data[i*8+4], rgba, sizeof(float)*4);	
			if (step + 1 < shade->steps->size) {
				UtilF *f = util_arr_get(shade->steps, step);
				if (i >= f->value * SHADE_SLIDE)
					step++;
				calculate_shade(i,step, rgba, drgba, shade);
			}

			for (j=0; j<4; j++)
				rgba[j] += drgba[j];
		}
		for (i=0; i<SHADE_SLIDE; i++)
			printf("%f %f %f %f\n", data[i*4], data[i*4+1], data[i*4+2], data[i*4+3]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, SHADE_SLIDE, 
			0, GL_RGBA, GL_FLOAT, data);
		g_hash_table_insert(shade2tex, GUINT_TO_POINTER(hash), GUINT_TO_POINTER(t));
		//printf("tex: %d %d\n", t, GPOINTER_TO_UINT(GUINT_TO_POINTER(t))); 
		tex = GUINT_TO_POINTER(t);
	}
	return GPOINTER_TO_UINT(tex);
}

static void rect(Rect *r, Coordsys *cs) {
	glColor3f(r->r, r->g, r->b);
	
	float 
		w = vob_coords_w(cs),
		h = vob_coords_h(cs);
	printf("rect: %2.1fx%2.1f %0.2f,%0.2f,%0.2f\n", w,h, r->r, r->g, r->b);

	glBegin(GL_QUADS);
	glVertex2f(0, h);
	glVertex2f(w, h);
	glVertex2f(w, 0);
	glVertex2f(0, 0);
	glEnd();
}

static void rounded_rect(VobRoundedRect *r, Coordsys *cs) 
{
	VobFill *fill = r->fill;
	//let's go for shortie
	VobShade *shade = (VobShade*)fill;
	float 
		w = vob_coords_w(cs),
		h = vob_coords_h(cs);

	glColor3f(0,0,1);
	//glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glBindTexture(GL_TEXTURE_2D, tex4shade(shade));

	glBegin(GL_QUADS);
	glTexCoord2f(0,1);
	glVertex2f(0, h);
	glTexCoord2f(1,1);
	glVertex2f(w, h);
	glTexCoord2f(1,0);
	glVertex2f(w, 0);
	glTexCoord2f(0,0);
	glVertex2f(0, 0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void gfx_opengl_vobs_render1(GHashTable *id2impl, 
			     Vob1 *v, Coordsys *cs)
{
	gpointer key, value;
	key = v->base.id;
	value = g_hash_table_lookup(id2impl, key);
	
	

	if (value == NULL) {
		if (strcmp(key, "vob.vobs.Rect") == 0)
			value = &rect;
		else if (strcmp(key, "vob.vobs.Glyph") == 0)
			value = &gfx_opengl_glyph;
		else if (strcmp(key, "vob.vobs.Paper") == 0)
			value = &gfx_opengl_paper;
		else if (strcmp(key, "vob.vobs.RoundedRect") == 0)
			value = &rounded_rect;

		if (value != NULL)
			g_hash_table_insert(id2impl, key, value);
		else {
			printf("No implementation for %s", (char *)key);
			abort();
		}
	}

	if (value != NULL && cs != NULL) {
		gfx_opengl_coords(cs);
		void (*func)(Vob1 *v, Coordsys *cs);
		func = value;
		func(v, cs);
	}
}
