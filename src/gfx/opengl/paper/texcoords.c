/*  texcoords.c -- texture coordinates
# 
# Copyright (c) 2003, Janne Kujala and Tuomas J. Lukka
#               2012, Matti Katila
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

#include <math.h>
#include "util/rng.h"
#include "gfx/opengl/paper/paper.h"

void gfx_opengl_paper_texcoords(GfxPaper *paper)
{
	double
		scale = 60.0, //.3,
		scale_log_stddev = 0.4,
		angle_stddev = .065,
		lendiff_mean = 0,
		lendiff_stddev = .1;
	
        // The angle between the basis vectors
	double angle = (.25 + angle_stddev*util_rng_next_gaussian()) * 2 * M_PI;
	angle *= 1 - 2 * util_rng_next_boolean();
	
	// The angle of the first basis vector
	double as = util_rng_next_double() * 2 * M_PI;
		// And the angle of the second basis vector
	double at = as + angle;

	// Logarightm of the random scale factor
        double m0 = scale_log_stddev * util_rng_next_gaussian();

	// The difference between basis vector lengths
	double m = lendiff_mean + lendiff_stddev * util_rng_next_gaussian();
	
	// The basis vector lengths
	double rs = scale * exp(m0 + m);
	double rt = scale * exp(m0 - m);
	
	// The vectors that give x and y when dotted with (s, t, 0, 1)
	//self.vecs = [[ rs * math.cos(as), rt * math.cos(at)],
        //             [ rs * math.sin(as), rt * math.sin(at)]]
	paper->rootrep[0][0] = rs * cos(as);
	paper->rootrep[0][1] = rt * cos(at);
	paper->rootrep[1][0] = rs * sin(as);
	paper->rootrep[1][1] = rt * sin(at);
}

