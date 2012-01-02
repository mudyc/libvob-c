/* color.c -- color utilities
# 
# Copyright (c) 2003, Janne Kujala and Tuomas J. Lukka
#               2011, Matti Katila
# 
# This file is part of libvob-c.
# 
# libvob-c is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# libvob-c is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
# Public License for more details.
# 
# You should have received a copy of the GNU General
# Public License along with libvob-c; if not, write to the Free
# Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA  02111-1307  USA
# 
# 
 */


#include "util/rng.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "gfx/opengl/paper/color.h"

// CIE1931 x,y-chromaticity coordinates of the
// red, green and blue phosphors of the monitor
// used in RGB <-> XYZ conversions
/*
static double R[] = {.64,.33,.03}; // these are the HDTV/sRGB/EBU/ITU primaries
static double G[] = {.30,.60,.10};
static double B[] = {.15,.06,.79};
*/


// Chromaticity coordinates of the white point (i.e., R = G = B = 1) --
// this determines the relative luminances of the primaries
// W = .312713,.329016 # = D65 standard illuminant
//static double W[] = {.3127268660,.3290235126, 0.3582496214 }; // = D65 standard 
                                                              // illuminant 
                                                              // [CIE 15.2, p.55]
static const double Wr = 0.212672955066; //(R[1] * det([W,G,B])) / (W[1] * det([R,G,B]))
static const double Wg = 0.715152296464; // (G[1] * det([R,W,B])) / (W[1] * det([R,G,B]))
static const double Wb = 0.0721747484701; //(B[1] * det([R,G,W])) / (W[1] * det([R,G,B]))



// The gamma and offset used for converting linear RGB to monitor RGB
static const double GAMMA = 2.4; //# sRGB standard
static const double OFFSET = .055; 


static double LtoY(double L) 
{
	/*
    Convert perceptual lightness (CIE L*) into linear luminance
    L: lightness between 0 and 100
    returns: luminance between 0 and 1
	*/
	if (L <= 8)
		return L * (27./24389);
	else
		return pow((L + 16.0) / 116, 3.0);
}
static double linear_to_monitor_f(double x) 
{
	if (x < 0) 
		return -linear_to_monitor_f(-x);
	if (OFFSET == 0)
		return pow(x, (1.0/GAMMA));

			     // Use a linear segment near zero
        double t = OFFSET / (GAMMA - 1);
	double t2 = pow((t * GAMMA / (1 + OFFSET)), GAMMA);
        double c = t / pow((t * GAMMA / (1 + OFFSET)), GAMMA);
        if (x < t2)
		return x * c;
        
        return pow(x, (1.0/GAMMA)) * (1 + OFFSET) - OFFSET;
}
static struct color *linear_to_monitor(struct color *c) 
{
	c->r = linear_to_monitor_f(c->r);
	c->g = linear_to_monitor_f(c->g);
	c->b = linear_to_monitor_f(c->b);
	return c;
}
static struct color *matvecmul(double mat[3][3], struct color *v) {
	v->r = mat[0][0] * v->r + mat[0][1] * v->g + mat[0][2] * v->b;
	v->g = mat[1][0] * v->r + mat[1][1] * v->g + mat[1][2] * v->b;
	v->b = mat[2][0] * v->r + mat[2][1] * v->g + mat[2][2] * v->b;
	return v;
}

static struct color *YSTtoRGB(struct color *c) 
{
	double mat[3][3] = {
		{ 1, Wg+Wb, (Wb - Wg) / sqrt(3) },
		{ 1,   -Wr, (2*Wb + Wr) / sqrt(3) },
		{ 1,   -Wr, -(2*Wg + Wr) / sqrt(3) }
	};
	matvecmul(mat, c);
	return c;
}
static struct color *RGBtoYST(struct color *c) 
{
	double mat[3][3] = {
		{ Wr, Wg, Wb },
		{ 1, -.5, -.5},
		{ 0, .5*sqrt(3), -.5*sqrt(3) }
	};
	matvecmul(mat, c);
	return c;
}
static double maxYSTsat(struct color *c) 
{
	// Split into "lightness" and "color" components
	struct color Y = { c->r, 0, 0};
	YSTtoRGB(&Y);
	struct color vec = { 0, c->g, c->b };
	RGBtoYST(&vec);

	double ret = fmin(((vec.r > 0) - Y.r) / vec.r,
			  ((vec.g > 0) - Y.g) / vec.g );
	ret = fmin(ret,   ((vec.b > 0) - Y.b) / vec.b );
	return ret;
}

static struct color *clamp_sat(struct color *c) 
{
	//"""Tests whether the vector is inside the unit cube [0,1]^n"""
	if ((c->r < 0 || c->r > 1) && (c->g < 0 || c->g > 1) && (c->b < 0 || c->b > 1))
		return c;

	RGBtoYST(c);
		
	double r = maxYSTsat(c);
	c->g *= r; 
	c->b *= r;
	
	YSTtoRGB(c);
	return c;
}

static void shuffle(int len, double *data)
{
        /*
        for i in reversed(xrange(1, len(x))):
 	  # pick an element in x[:i+1] with which to exchange x[i]
 	  j = int(random() * (i+1))
 	  x[i], x[j] = x[j], x[i]
	*/
	int i;
	for (i=len-1; i>=1; i--) {
		int j = (int) (util_rng_next_double() * (i+1));
		double tmp = data[i];
		data[i] = data[j];
		data[j] = tmp;
	}
}
static void shuffle_color(int len, struct color *data)
{
	int i;
	for (i=len-1; i>=1; i--) {
		int j = (int) (util_rng_next_double() * (i+1));
		struct color tmp = data[i];
		data[i] = data[j];
		data[j] = tmp;
	}
}

void gfx_opengl_paper_create_colors(unsigned long seed, int colors, int minlum, 
				    int blend, struct color *ret) 
{
	int i;
	util_rng_set_seed(seed);
	double huerange = util_rng_next_gaussian() * 90;

	
	// Sample hues uniformly from the range shifted to a random angle
	double hue0 = util_rng_next_double();
	double hues[colors];
	hues[0] = hue0;
	hues[1] = hue0 + huerange;
	for (i=2; i<colors; i++)
		hues[i] = hue0 + util_rng_next_double() * huerange;

	// map angles
	int angles[] = { 0, 30, 60, 120, 180, 240, 300, 360 };
	for (i=0; i<colors; i++) {
		double f = hues[i];
		f *= 7.0 / 360.0;
		int index = ((int)f) % 7;
		double fract = f - (int) f;
		hues[i] = (1- fract) * angles[index] + fract * angles[index + 1];
	}
	shuffle(colors, hues);
	
	// Take one half dark colors and one half light colors
        int lumrange = 100 - minlum;
	double lums[colors];
	for (i=0; i<colors/2; i++)
		lums[i] = minlum + util_rng_next_double() * lumrange/2;
	for (; i<colors; i++)
		lums[i] = minlum + (1 + util_rng_next_double()) * lumrange/2;

	/* Sample saturation:
          - take the most saturated color 2/3 of the time
            and a dull color 1/3 of the time
	*/
	double sats[colors];
	for (i=0; i<colors; i++) {
		double tmp = 1 - util_rng_next_double();
		tmp *= tmp;
		sats[i] = (1 - (1 - tmp) * (util_rng_next_double() < .333));
	}
	
	// Construct colors and clamp to RGB cube keeping hue and 
	// luminance constant
	struct color yst[colors];
	for (i=0; i<colors; i++) {
		yst[i].r = LtoY(lums[i]);
		yst[i].g = sats[i] * cos(hues[i]*M_PI/180.);
		yst[i].b = sats[i] * sin(hues[i]*M_PI/180.);
	}
	for (i=0; i<colors; i++)
		linear_to_monitor(clamp_sat(YSTtoRGB(&yst[i])));
	shuffle_color(colors, yst);

	memcpy(ret, yst, sizeof(yst));
}
