#include <string.h>
#include <stdio.h>
#include "gfx/opengl/coords.h"
#include "gfx/opengl/vobs.h"
#include "gfx/opengl/util.h"
#include "vob/coords.h"
#include "vob/vobs/rect.h"
#include "util/primitives.h"


static GLuint round_tex;
/*-
 *  ^
 *  |
 *  |..
 *  |  ''   0
 *  |     '
 *  | 1    |
 *  +------------------>
 */
static void generate_round_tex()
{
	int x,y;
	int W = 64;
	float data[W][W];
	glGenTextures(1, &round_tex);
	glBindTexture(GL_TEXTURE_2D, round_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	for (y=0; y<W; y++) {
		for (x=0; x<W; x++) {
			// a^2 + b^2 = c^2
			double c2 = x*x + y*y;
			double c = sqrt(c2);
			if (c <= W)
				data[y][x] = 1;
			else if (c >= W + 1) 
				data[y][x]= 0;
			else
				data[y][x] = 1. - (c - W);
		}
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, W,W, 0, GL_ALPHA, GL_FLOAT, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	glError();
}


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
	//printf("i %d, j %d, d %d, step %d\n", i, j, d, step);
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
		generate_round_tex();
	}

	guint hash = shade_hash(shade);
	gpointer tex = g_hash_table_lookup(shade2tex, GUINT_TO_POINTER(hash));
	if (tex == NULL) {
		GLuint t;
		// rgba * min w == 2 * slide height
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
				if (i >= f->value * SHADE_SLIDE) {
					step++;
					calculate_shade(i,step, rgba, drgba, shade);
				}
			}

			for (j=0; j<4; j++)
				rgba[j] += drgba[j];
		}
		/*
		for (i=0; i<SHADE_SLIDE; i+=2)
			printf("%0.2f %0.2f %0.2f %1.0f\n", data[i*4], data[i*4+1], data[i*4+2], data[i*4+3]);
		*/
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, SHADE_SLIDE, 
			0, GL_RGBA, GL_FLOAT, data);
		glBindTexture(GL_TEXTURE_2D, 0);
		g_hash_table_insert(shade2tex, GUINT_TO_POINTER(hash), GUINT_TO_POINTER(t));
		//printf("tex: %d %d\n", t, GPOINTER_TO_UINT(GUINT_TO_POINTER(t))); 
		tex = GUINT_TO_POINTER(t);
		glError();
	}
	return GPOINTER_TO_UINT(tex);
}

void gfx_opengl_rounded_rect(VobRoundedRect *r, Coordsys *cs) 
{
	//printf("gfx opengl rounded rect\n");

	VobFill *fill = r->fill;
	//let's go for shortie
	VobShade *shade = (VobShade*)fill;
	float 
		w = vob_coords_w(cs),
		h = vob_coords_h(cs);

	glError();
	glColor3f(0,0,1);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_ALWAYS, 0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glError();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex4shade(shade));
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glError();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, round_tex);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

	float xr = (w/2 - r->r) / -r->r;
	float xt = (h/2 - r->t) / -r->t;
	float xl = (w/2 - r->l) / -r->l;
	float xb = (h/2 - r->b) / -r->b;

	glBegin(GL_QUADS);
	// up right part..
	// 0,0
	glMultiTexCoord2f(GL_TEXTURE0, 0, 0.5);
	glMultiTexCoord2f(GL_TEXTURE1, xr, xt);
	glVertex2f(w/2, h/2);
	// 1,0
	glMultiTexCoord2f(GL_TEXTURE0, 1, 0.5);
	glMultiTexCoord2f(GL_TEXTURE1, 1, xt);
	glVertex2f(w, h/2);
	// 1,1
	glMultiTexCoord2f(GL_TEXTURE0, 1, 1);
	glMultiTexCoord2f(GL_TEXTURE1, 1, 1);
	glVertex2f(w, 0);
	// 0,1
	glMultiTexCoord2f(GL_TEXTURE0, 0, 1);
	glMultiTexCoord2f(GL_TEXTURE1, xr, 1);
	glVertex2f(w/2, 0);

	// up left part..
	// 0,0
	glMultiTexCoord2f(GL_TEXTURE0, 0, 0.5);
	glMultiTexCoord2f(GL_TEXTURE1, 1, xt);
	glVertex2f(0, h/2);
	// 1,0
	glMultiTexCoord2f(GL_TEXTURE0, 1, 0.5);
	glMultiTexCoord2f(GL_TEXTURE1, xl, xt);
	glVertex2f(w/2, h/2);
	// 1,1
	glMultiTexCoord2f(GL_TEXTURE0, 1, 1);
	glMultiTexCoord2f(GL_TEXTURE1, xl, 1);
	glVertex2f(w/2, 0);
	// 0,1
	glMultiTexCoord2f(GL_TEXTURE0, 0, 1);
	glMultiTexCoord2f(GL_TEXTURE1, 1, 1);
	glVertex2f(0, 0);

	// down left part..
	// 0,0
	glMultiTexCoord2f(GL_TEXTURE0, 0, 0);
	glMultiTexCoord2f(GL_TEXTURE1, 1, 1);
	glVertex2f(0, h);
	// 1,0
	glMultiTexCoord2f(GL_TEXTURE0, 1, 0);
	glMultiTexCoord2f(GL_TEXTURE1, xl, 1);
	glVertex2f(w/2, h);
	// 1,1
	glMultiTexCoord2f(GL_TEXTURE0, 1, 0.5);
	glMultiTexCoord2f(GL_TEXTURE1, xl, xb);
	glVertex2f(w/2, h/2);
	// 0,1
	glMultiTexCoord2f(GL_TEXTURE0, 0, 0.5);
	glMultiTexCoord2f(GL_TEXTURE1, 1, xb);
	glVertex2f(0, h/2);

	// down right part..
	// 0,0
	glMultiTexCoord2f(GL_TEXTURE0, 0, 0);
	glMultiTexCoord2f(GL_TEXTURE1, xl, 1);
	glVertex2f(w/2, h);
	// 1,0
	glMultiTexCoord2f(GL_TEXTURE0, 1, 0);
	glMultiTexCoord2f(GL_TEXTURE1, 1, 1);
	glVertex2f(w, h);
	// 1,1
	glMultiTexCoord2f(GL_TEXTURE0, 1, 0.5);
	glMultiTexCoord2f(GL_TEXTURE1, 1, xb);
	glVertex2f(w, h/2);
	// 0,1
	glMultiTexCoord2f(GL_TEXTURE0, 0, 0.5);
	glMultiTexCoord2f(GL_TEXTURE1, xl, xb);
	glVertex2f(w/2, h/2);

	glEnd();

	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	glError();
}
