/*
fnoise.texture
 *    
 *    Copyright (c) 2003, Janne Kujala and Tuomas J. Lukka
 *    
 *    This file is part of Gzz.
 *    
 *    Gzz is free software; you can redistribute it and/or modify it under
 *    the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *    
 *    Gzz is distributed in the hope that it will be useful, but WITHOUT
 *    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 *    Public License for more details.
 *    
 *    You should have received a copy of the GNU General
 *    Public License along with Gzz; if not, write to the Free
 *    Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *    MA  02111-1307  USA
 *    
 *    
 */
/*
 * Written by Janne Kujala and Tuomas J. Lukka
 */

#include "gfx/opengl/paper/texture.h"

// -*-C++-*-
#include <math.h>
#include <stdlib.h>

#define FPARAM(name, default) \
	float name = default; \
	if (params->g_##name != 0) name = params->g_##name;

#define a b

static double drand() {
    return random() / (double)RAND_MAX;
}

/* Add a fourier noise to the data.
 */
static void fourier_noise(int width, int height, int depth, int components, float *data, 
			  float freq, float df, float aniso/* = 1.0*/) {

	int i,f,j;

    int nf = (int)(2 * (freq + df) * aniso);
    float xsin[2][nf][width];
    for(i=0; i<width; i++) 
	for(f = 0; f < nf; f++) {
	    xsin[0][f][i] = sin(i/(float)width * f * M_PI * 2);
	    xsin[1][f][i] = cos(i/(float)width * f * M_PI * 2);
    }
    float ysin[2][nf][height];
    for(j=0; j<width; j++) 
	for(f = 0; f < nf; f++) {
	    ysin[0][f][j] = sin(j/(float)height * f * M_PI * 2);
	    ysin[1][f][j] = cos(j/(float)height * f * M_PI * 2);
    }
    
    float sumsq[components];
    for(i=0; i<components; i++) sumsq[i] = 0;
    
    if(depth < 2) {
	// 2D
        int xf, yf, xsc, ysc, co; 
	for(xf = 0; xf < nf; xf++) {
	  for(yf = 0; yf < nf; yf++) {

	    double f = sqrt(xf*xf + yf*yf*(aniso*aniso));
	    if(f < freq-df || f > freq+df) continue;

	    for(xsc = 0; xsc < 2; xsc++) {
	      for(ysc = 0; ysc < 2; ysc++) {
		float coeff[components];
		for(co = 0; co < components; co++) {
		    coeff[co] = drand()-0.5;
		    sumsq[co] += coeff[co] * coeff[co];
		}
		int ind = 0;
		for (j = 0; j < height; j++) {
		  for (i = 0; i < width; i++) {
		    for(co = 0; co < components; co++)
			data[ind++] += xsin[xsc][xf][i] * ysin[ysc][yf][j] * coeff[co];
		  }
		}
	      }
	    }
	  }
	}
	int ind = 0;

	for(co = 0; co < components; co++)
	  sumsq[co] = sqrt(sumsq[co]);
	
	for (j = 0; j < height; j++) {
	  for (i = 0; i < width; i++) {
	    for(co = 0; co < components; co++) {
		data[ind++] /= sumsq[co];
	    }
	  }
	}
	
    } else {
	// float zsin[width][nf];
	// 3D
    }
}

double identity(double x) { return x; }

void gfx_opengl_generate_texture_fnoise(struct texture *params, float *data) {
	int width = params->w;
	int height = params->h;
	int depth = 1;
	int components = params->texture_components;
//void GENERATE(TextureParam *params, int width, int height, int depth, int components, float *data) {
    FPARAM(bias, 0);
    FPARAM(scale, 1);
    FPARAM(freq, 5);
    FPARAM(aniso, 1.0);
    FPARAM(df, 2);
    FPARAM(seed, 0);
    FPARAM(turb, 0);
    FPARAM(fbm, 0);
    FPARAM(freq2, 20);

    int i; float f;
    
    double (*func)(double) = identity;
    if (turb) func = fabs, fbm = 1;

    if (seed) srandom((long)seed);

    int d = (depth==0 ? 1 : depth);
    int n = width*height*d*components;

    for(i = 0; i<n; i++)
      data[i] = 0;


    if (fbm) {
      float tmp[n];
      
      for (f = freq; f <= freq2; f += f) {
	for(i = 0; i<n; i++)
	  tmp[i] = 0;
	fourier_noise(width,height,d,components,tmp, f, df, aniso);

	float m = 1.0 / (log(f)/log(2) + 1);
	
	for(i = 0; i<n; i++)
	  data[i] += m * func(tmp[i]);
      }
      
      for(i = 0; i<n; i++)
	tmp[i] = 0;

    } else {

      fourier_noise(width,height,d,components,data, freq, df, aniso);
    }
    
    for(i = 0; i<n; i++) {
      data[i] *= scale;
      data[i] += bias;
    }
}

// vim: set syntax=c :
