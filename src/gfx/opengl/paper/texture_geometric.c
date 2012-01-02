/*
geometric.texture
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

#define SQR(A) ((A) * (A))

float pyramid(float x, float y) {
  x -= floor(x);
  y -= floor(y);

  x = fabs(x - .5);
  y = fabs(y - .5);

  return 1 - 2 * (x > y ? x : y);
}

float checkerboard(float x, float y) {
  x -= floor(x);
  y -= floor(y);

  return (x < .499999) ^ (y < .499999);
}

float cone(float x, float y) {
  x -= floor(x);
  y -= floor(y);

  x -= .5;
  y -= .5;
  
  return 1 - 2 * sqrt(.5 * (x * x + y * y));
}

float checkerboard2(float x, float y) {
  x -= floor(x);
  y -= floor(y);
  return (x < .499999) && (y < .499999);
}

float saw(float x) {
  return x - floor(x);
}

float triangle(float x) {
  x -= floor(x);
  return 1 - 2 * fabs(x - .5);
}

float stripe(float x) {
  x -= floor(x);
  return (x < .4999999);
}

float circle(float x, float y) {
  return ((SQR(x - 0.5) + SQR(y - 0.5)) <= SQR(0.5));
}

void gfx_opengl_generate_texture_geometric(struct texture *params, float *data) {
	int width = params->w;
	int height = params->h;
	int depth = 1;
	int components = params->texture_components;
//void GENERATE(TextureParam *params, int width, int height, int depth, int components, float *data) {
    FPARAM(type, 0);
    FPARAM(scale, 1);
    FPARAM(bias, 0);
    FPARAM(seed, 1);
    if(components > 4) return;
    
    if (seed != 1) {
      srandom((unsigned)seed);
    }

    int ind=0, i, j;
    float x, y;
    float xstep = 1.0 / width;
    float ystep = 1.0 / height;

    for (j = 0, y = 0; j < height; j++, y += ystep) {	
      for (i = 0, x = 0; i < width; i++, x += xstep) {

	switch ((int)type) {
	case 0: 
	  if (components >= 1) data[ind++] = pyramid(x, y);
	  if (components >= 2) data[ind++] = pyramid(x + .5, y);
	  if (components >= 3) data[ind++] = pyramid(x, y + .5);
	  if (components >= 4) data[ind++] = pyramid(x + .5, y + .5);
	  break;
	case 1:
	  if (components >= 1) data[ind++] = checkerboard(x, y);
	  if (components >= 2) data[ind++] = checkerboard(x + .25, y);
	  if (components >= 3) data[ind++] = checkerboard(x, y + .25);
	  if (components >= 4) data[ind++] = checkerboard(x + .25, y + .25);
	  break;
	case 2:
	  if (components >= 1) data[ind++] = cone(x, y);
	  if (components >= 2) data[ind++] = cone(x + .5, y);
	  if (components >= 3) data[ind++] = cone(x, y + .5);
	  if (components >= 4) data[ind++] = cone(x + .5, y + .5);
	  break;
	case 3:
	  if (components >= 1) data[ind++] = checkerboard2(x, y);
	  if (components >= 2) data[ind++] = checkerboard2(x + .5, y);
	  if (components >= 3) data[ind++] = checkerboard2(x, y + .5);
	  if (components >= 4) data[ind++] = checkerboard2(x + .5, y + .5);
	  break;
	case 4:
	  if (components >= 1) data[ind++] = saw(x);
	  if (components >= 2) data[ind++] = saw(y);
	  if (components >= 3) data[ind++] = saw(1 - xstep - x);
	  if (components >= 4) data[ind++] = saw(1 - ystep - y);
	  break;
	case 5:
	  if (components >= 1) data[ind++] = triangle(x);
	  if (components >= 2) data[ind++] = triangle(y);
	  if (components >= 3) data[ind++] = triangle(.5 + x);
	  if (components >= 4) data[ind++] = triangle(.5 + y);
	  break;
	case 6:
	  if (components >= 1) data[ind++] = stripe(x);
	  if (components >= 2) data[ind++] = stripe(y);
	  if (components >= 3) data[ind++] = stripe(1 - xstep - x);
	  if (components >= 4) data[ind++] = stripe(1 - ystep - y);
	  break;
	case 7:
	  if (components >= 1) data[ind++] = rand() / (RAND_MAX + 1.0);
	  if (components >= 2) data[ind++] = rand() / (RAND_MAX + 1.0);
	  if (components >= 3) data[ind++] = rand() / (RAND_MAX + 1.0);
	  if (components >= 4) data[ind++] = rand() / (RAND_MAX + 1.0);
	  break;
	case 8:
	  if (components >= 1) data[ind++] = circle(x + 0.5/width, y + 0.5/height);
	  if (components >= 2) data[ind++] = circle(x + 0.5/width, y + 0.5/height);
	  if (components >= 3) data[ind++] = circle(x + 0.5/width, y + 0.5/height);
	  if (components >= 4) data[ind++] = circle(x + 0.5/width, y + 0.5/height);
	  break;
      	}
      }	
    }

    for(i = 0; i < width * height * depth * components; i++) {
      data[i] = data[i] * scale + bias;
    }
    
}



