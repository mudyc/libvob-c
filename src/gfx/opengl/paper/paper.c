/* paper.c  -- paper background renderer

   Copyright (c) 2003, Janne Kujala and Tuomas J. Lukka
                 2012 Matti Katila

   libvob-c is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   libvob-c is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with libvob-c; see the file COPYING.  If not, write to the
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301 USA.

*/


#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#include "vob/coords.h"
#include "gfx/opengl/paper/texture.h"
#include "gfx/opengl/paper/texcoords.h"
#include "gfx/opengl/paper/shaders.h"
#include "gfx/opengl/paper/paper.h"
#include "gfx/opengl/util.h"
#include "vob/vobs/paper.h"
#include "util/rng.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


static void select_textures(GfxPaper *paper, unsigned long seed)
{
	util_rng_set_seed(seed);
	double hyper = util_rng_next_gaussian()*5;
	int i;
	int count = gfx_opengl_paper_texture_count();
	double probs[count];
	double sum = 0;
	for (i=0; i<count; i++) {
		probs[i] = exp(util_rng_next_gaussian() * hyper);
		sum += probs[i];
	}
	for (i=0; i<count; i++) {
		probs[i] = probs[i] / sum;
		paper->tex_probs[i] = probs[i];
	}
}

static int get_paper_texture(GfxPaper *paper) {
	double t = util_rng_next_double();
	int count = gfx_opengl_paper_texture_count();
	int i;
	for (i=0; i<count; i++) {
		t -= paper->tex_probs[i];
		if (t < 0)
			return i;
		
	}
	return 0; //gfx_opengl_paper_texture_get(0);
}

static void get_paper() {
	// colors

	// textures

}

static void makePaperPass()
{
//reg, ppass, colors, textures, rootrep, trans = 0, emboss = 0):
/*
        self.selectCombiner()
        seed = reg.get(regseed, "seed")
        rnd = java.util.Random(seed)
*/
	/*if emboss:
            sh = makeEmbossShaderPass()
        else:
            sh = makeNormalShaderPass()
	*/
	/*
        types = sh.getTextureTypes()
        for i in range(0, len(types)):
            if types[i] != None:
                t = textures.getPaperTexture(types[i], rnd)

                if emboss:
                    if (i%2) == 0:
                        prev = t
                    else:
                        t = prev
                    
                if dbg: print "Texture"+str(i)+":", t.getName()
                    
                sh.setTexture(i, t)
        if emboss:
            comb = EmbossCombinerPass()
        else:
            #comb = self.DebugCombinerPass()
            comb = self.TransparentCombinerPass()
	*/
/*
        code = """
            PushAttrib ENABLE_BIT TEXTURE_BIT DEPTH_BUFFER_BIT COLOR_BUFFER_BIT CURRENT_BIT
            Disable BLEND
        """

        # Performance optimization:
        # Set normal depth testing for the first (non-transparent) pass
        # and "equal" depth testing for the latter (transparent) passes,
        # and don't write to the depth buffer.

        if trans == 0 and emboss == 0:
            code += """
                Enable DEPTH_TEST
                DepthFunc LESS
            """
        else:
            code += """
                Enable DEPTH_TEST
                DepthFunc EQUAL
                DepthMask 0
            """
                
        code += sh.setupCode(rnd)
        code += comb.setupCode(sh.getRGBoutputs(), sh.getRGBoutputscales(),
                               colors, rnd, trans)

        ppass.setSetupcode(code)
        if dbg: print "SETUP: ", code
        ppass.setTeardowncode("""
            PopAttrib
            ActiveTexture TEXTURE0
        """)

        eps = 1E-4
        
        ttyp = sh.getTexgenTypes()
        ppass.setNTexGens(len(ttyp))

        for i in range(0, len(ttyp)):
            if ttyp[i] != None:
                sca2 = 0.8

                if ttyp[i] in ("TexGen2D", "TexGen3D"):
                    data = rootrep.getRelated(rnd).texCoords2D(rnd).getVec()
                elif ttyp[i] == "TexGen2D":
                    data = TexCoords().texCoords2D(rnd).getVec()
                elif ttyp[i] == "TexGen3D":
                    data = TexCoords().texCoords3D(rnd).getVec()
                    
                elif ttyp[i] == "TexGenDotVector":
                    data = [ 0, 0, 0, sca2*rnd.nextGaussian(), 
                             0, 0, 0, sca2*rnd.nextGaussian(),
                             0, 0, 0, sca2*rnd.nextGaussian() ]
                else:
                    assert 0
                if len(data) < 12:
                    for i in (0,0,0,1): data.append(i)
                if emboss :
                    if eps > 0:
                        prev = data
                    else:
                        data = prev
                    if dbg: print "TexGenEmboss"+str(i), data, eps
                    ppass.putEmbossTexGen(i, prev, eps)
                    eps = -eps
                else:
                    if dbg: print "TexGen"+str(i), data
                    ppass.putNormalTexGen(i, data)
*/
}

//rootrep = TexGenXYRepeatUnit(rng, scale = 60 * scaleFactor, vecs = vecs)
/*   def __init__(self, rnd=None, 
	    vecs = None,
	    scale = .3,
	    scale_log_stddev = 0.4,
	    angle_stddev = .065,
	    lendiff_mean = 0,
	    lendiff_stddev = .1):*/
static void create_texgen_coords(GfxPaper *paper, int pass) {
	int texgen;
	for (texgen=0; texgen<2; texgen++) {
		int a,b,c,d;
		while (1) {
			a = (int)2*pow(util_rng_next_gaussian(), 3);
			b = (int)2*pow(util_rng_next_gaussian(), 3);
			c = (int)2*pow(util_rng_next_gaussian(), 3);
			d = (int)2*pow(util_rng_next_gaussian(), 3);
			double l1 = sqrt(a*a + b*b);
			double l2 = sqrt(c*c + d*d);
			double det = a*d-b*c;
			if (det == 0) continue;
			if (((l1 * l2) / det) > 2) continue;
			if (abs(log(l1 / l2)) > .7) continue;
			break;
		}
		// 1 / determinant
		double f = 1.0 / (a * d - b * c);
		double mat[2][2];
		mat[0][0]
			= f * d * paper->rootrep[0][0] -
			f * b * paper->rootrep[0][1];
		mat[0][1]
			= -f * c * paper->rootrep[0][0] +
			f * a * paper->rootrep[0][1];
		mat[1][0]
			= f * d * paper->rootrep[1][0] -
			f * b * paper->rootrep[1][1];
		mat[1][1]
			= -f * c * paper->rootrep[1][0] +
			f * a * paper->rootrep[1][1];
		
		paper->pass[pass].texgenrep[texgen][0][0] = f * mat[1][1];
		paper->pass[pass].texgenrep[texgen][0][1] = -f * mat[0][1];
		paper->pass[pass].texgenrep[texgen][1][0] = -f * mat[1][0];
		paper->pass[pass].texgenrep[texgen][1][1] = f * mat[0][0];
		
		paper->pass[pass].texgenrep[texgen][0][2] = 0;
		paper->pass[pass].texgenrep[texgen][1][2] = 0;
		paper->pass[pass].texgenrep[texgen][0][3] = 
			util_rng_next_double();
		paper->pass[pass].texgenrep[texgen][1][3] = 
			util_rng_next_double();
	}
}

GfxPaper *gfx_opengl_paper_create(unsigned long seed)
{
	GfxPaper *ret = malloc(sizeof(GfxPaper));
	ret->pass_count = 3;
	ret->pass[0].transparency = 0;
	ret->pass[1].transparency = .5;
	ret->pass[2].transparency = .9375;

	util_rng_set_seed(seed);
	int i;
	// eat bad beginning (Java's bad PRNG)
	for (i=0; i<20; i++) 
		util_rng_next_int(); 

	//generate colors
	gfx_opengl_paper_create_colors(util_rng_next_int(), 8, 80, 0, ret->colors);
	for (i=0; i<8; i++) 
		printf("%d %ld color %.2f %.2f %.2f\n", 
		       i, seed, ret->colors[i].r, ret->colors[i].g, ret->colors[i].b);

	// select textures to be used
	select_textures(ret, util_rng_next_int());

	//make repeating units
	gfx_opengl_paper_texcoords(ret);

	for (i=0; i<ret->pass_count; i++)
		ret->pass[i].seed = util_rng_next(2000000000);

	// make paper pass init
	int pass;
	for(pass = 0; pass < ret->pass_count; pass++) {
		util_rng_set_seed(ret->pass[pass].seed);
		int texgen;
		for (texgen=0; texgen < 2; texgen++) {
			i = get_paper_texture(ret);
			ret->pass[pass].textures[texgen] = 
				gfx_opengl_paper_texture_get(i);
			ret->pass[pass].textures_scale[texgen] = 
				gfx_opengl_paper_texture_scale_get(i);
		}

		// set combiner
		// colors
		// combiner path type
		ret->pass[pass].path = util_rng_next_int_max(3);

		// Random scaling of (dot) products
		double rndscale;
		if (ret->pass[pass].transparency > 0) {
			// Try to keep the textures non-fuzzy
			rndscale = exp(.3*abs(util_rng_next_gaussian()));
		} else {
			rndscale = exp(.5*util_rng_next_gaussian());
		}

		double avgscale = (ret->pass[pass].textures_scale[0] + 
				   ret->pass[pass].textures_scale[1]) / 2.0;
		double scale, /*band*/ alphascale;
		if (ret->pass[pass].path == 0) {
			scale = avgscale * 8.0 * rndscale;
			/*band*/alphascale = 3.0 * exp(.5 * util_rng_next_gaussian());
		} else if (ret->pass[pass].path == 1) {
			scale = avgscale * 8.0 * rndscale;
			alphascale = exp(.5 * abs(util_rng_next_gaussian()));
		} else {
			scale = avgscale * 8.0 * rndscale;
			alphascale = avgscale * 8.0 * rndscale;
		}
		ret->pass[pass].scale = scale;
		ret->pass[pass].band_or_alphascale = alphascale;

		create_texgen_coords(ret, pass);
	}

	return ret;
}


void gfx_opengl_paper(Vob1 *v, Coordsys *cs)
{
	static bool init = false;
	if (!init) {
		printf("create textures\n");
		gfx_opengl_paper_create_textures();
		gfx_opengl_check_error("texture init");

		printf("create shaders\n");
		gfx_opengl_paper_shaders_init();
		gfx_opengl_check_error("shader init");

		init = true;
		printf("created\n");
	}

	unsigned long seed = ((Paper *)v)->id;
	GfxPaper *paper = gfx_opengl_paper_create(seed);

	float 
		w = vob_coords_w(cs),
		h = vob_coords_h(cs);

	//if (true) return;

	glPushAttrib(GL_ENABLE_BIT 
		     | GL_TEXTURE_BIT 
		     | GL_DEPTH_BUFFER_BIT 
		     | GL_COLOR_BUFFER_BIT 
		     | GL_CURRENT_BIT);
	glDisable(GL_BLEND);

	int pass;
	for(pass = 0; pass < 1; /*paper->pass_count;*/ pass++) {
		
		//Pass pass = paper.getPass(it);
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, paper->pass[pass].textures[0]);

		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, paper->pass[pass].textures[1]);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);
		glDisable(GL_ALPHA_TEST);

		/*
		Enable REGISTER_COMBINERS_NV
	    CombinerParameterNV NUM_GENERAL_COMBINERS_NV 2
	    
	    CombinerParameterNV CONSTANT_COLOR0_NV %(r0)s
	    CombinerParameterNV CONSTANT_COLOR1_NV %(r1)s
	    Color %(c0)s
	    SecondaryColorEXT %(c1)s
	    Fog FOG_COLOR %(c2)s
		*/
		gfx_opengl_check_error("paper pass start");

		glUseProgram(gfx_opengl_paper_shaders_get(0));

		// set colors
		//glUniform3fvARB(g_programCameraPositionLocation, 1, g_cameraPosition);
		//glUniform3fvARB(g_programLightPositionLocation, NUM_LIGHTS, g_lightPosition);
		//glUniform3fvARB(g_programLightColorLocation, NUM_LIGHTS, g_lightColor);

		
		glBegin(GL_QUADS);

		glVertex2f(0, h);
		glVertex2f(w, h);
		glVertex2f(w, 0);
		glVertex2f(0, 0);

		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glUseProgram(0);
	
	gfx_opengl_check_error("paper done");
	
	glPopAttrib();
	glActiveTexture(GL_TEXTURE0);
}
