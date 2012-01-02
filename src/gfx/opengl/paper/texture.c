/* texture.c -- texture generation
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



#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "gfx/opengl/util.h"
#include "gfx/opengl/paper/texture.h"

#define tres 128
#define text_comps 4
#define text_form GL_RGBA

static struct texture texts[] = {
	{
		.name = "rgbw1",
		.w = tres, 
		.h = tres,
		.texture_components = text_comps,
		.texture_format = text_form,
		.text_name = FNOISE,
		.g_scale = .43f,
		.g_freq = 1.f,
		.g_df = .3f,
		.g_bias = 0.5f,
		.g_seed = 2323,
		.scale = 1.f/2.f,
		.featurescale = 1.f/2.f
	},
	{
		.name = "rgbw2",
		.w = tres, 
		.h = tres,
		.texture_components = text_comps,
		.texture_format = text_form,
		.text_name = FNOISE,
		.g_scale = .43f,
		.g_freq = 1.f,
		.g_df = 1.2f,
		.g_bias = 0.5f,
		.scale = 1.f/2.f,
		.featurescale = 1.f/4.f
	},
	{
		.name = "rgbw3",
		.w = tres, 
		.h = tres,
		.texture_components = text_comps,
		.texture_format = text_form,
		.text_name = FNOISE,
		.g_scale = .3f,
		.g_freq = 1.f,
		.g_df = 1.9f,
		.g_bias = 0.5f,
		.g_seed = 361,
		.g_aniso = 2.f,
		.scale = 1.f/2.f,
		.featurescale = 1.f/4.f
	},
	{
		.name = "turb",
		.w = tres, 
		.h = tres,
		.texture_components = text_comps,
		.texture_format = text_form,
		.text_name = FNOISE,
		.g_turb = 1.f,
		.g_scale = .3f,
		.g_freq = 1.f,
		.g_freq2 = 100,
		.g_df = 1.9f,
		.g_bias = 0.5f,
		.scale = 1.f/8.f,
		.featurescale = 1.f/16.f
	},
	{
		.name = "pyramid",
		.w = 64, 
		.h = 64,
		.texture_components = text_comps,
		.texture_format = text_form,
		.text_name = GEOMETRIC,
		.g_type = 0,
		.scale = 1.f/2.f,
		.featurescale = 1.f/2.f
	},
	{
		.name = "checkerboard",
		.w = 4, 
		.h = 4,
		.texture_components = text_comps,
		.texture_format = text_form,
		.text_name = GEOMETRIC,
		.g_type = 1,
		.g_scale = .5f,
		.g_bias = .5f,
		.continuous = 0,
		.scale = 1.f/8.f,
		.featurescale = 1.f/4.f,
		.minfilter = GL_NEAREST,
		.magfilter = GL_NEAREST
		},
	{
		.name = "cone",
		.w = 64, 
		.h = 64,
		.texture_components = text_comps,
		.texture_format = text_form,
		.text_name = GEOMETRIC,
		.g_type = 2,
		.scale = 1.f/2.f,
		.featurescale = 1.f/2.f,
	},
	{
		.name = "checkerboard2",
		.w = 2, 
		.h = 2,
		.texture_components = text_comps,
		.texture_format = text_form,
		.text_name = GEOMETRIC,
		.g_type = 3,
		.g_scale = .5f,
		.g_bias = .5f,
		.continuous = 0,
		.scale = 1.f/8.f,
		.featurescale = 1.f/2.f,
		.minfilter = GL_NEAREST,
		.magfilter = GL_NEAREST
	},
	{
		.name = "saw",
		.w = 64, 
		.h = 64,
		.texture_components = text_comps,
		.texture_format = text_form,
		.text_name = GEOMETRIC,
		.g_type = 4,
		.continuous = 0,
		.scale = 1.f/8.f,
		.featurescale = 1.f,
		.minfilter = GL_NEAREST,
		.magfilter = GL_NEAREST
	},
	{
		.name = "triangle",
		.w = 64, 
		.h = 64,
		.texture_components = text_comps,
		.texture_format = text_form,
		.text_name = GEOMETRIC,
		.g_type = 5,
		.scale = 1.f/2.f,
		.featurescale = 1.f/2.f,
	},
	{
		.name = "stripe",
		.w = 2, 
		.h = 2,
		.texture_components = text_comps,
		.texture_format = text_form,
		.text_name = GEOMETRIC,
		.g_type = 6,
		.g_scale = .5f,
		.g_bias = .5f,
		.continuous = 0,
		.scale = 1.f/8.f,
		.featurescale = 1.f/2.f,
		.minfilter = GL_NEAREST,
		.magfilter = GL_NEAREST
	},
	{
		.name = "rnd0",
		.w = 2, 
		.h = 2,
		.texture_components = text_comps,
		.texture_format = text_form,
		.text_name = GEOMETRIC,
		.g_type = 7,
		.g_seed = 10,
		.continuous = 0,
		.scale = 1.f/2.f,
		.featurescale = 1.f/2.f,
	},
	{
		.name = "rnd1",
		.w = 2, 
		.h = 4,
			.texture_components = text_comps,
		.texture_format = text_form,
		.text_name = GEOMETRIC,
		.g_type = 7,
			.g_seed = 11,
		.continuous = 0,
		.scale = 1.f/4.f,
		.featurescale = 1.f/4.f,
	},
	{
		.name = "rnd2",
		.w = 4, 
		.h = 4,
		.texture_components = text_comps,
		.texture_format = text_form,
		.text_name = GEOMETRIC,
		.g_type = 7,
		.g_seed = 12,
		.continuous = 0,
		.scale = 1.f/4.f,
		.featurescale = 1.f/4.f,
	},
	{
		.name = "rnd0n",
		.w = 2, 
		.h = 2,
		.texture_components = text_comps,
		.texture_format = text_form,
		.text_name = GEOMETRIC,
		.g_type = 7,
		.g_seed = 13,
		.continuous = 0,
		.scale = 1.f/2.f,
		.featurescale = 1.f/2.f,
		.minfilter = GL_NEAREST,
		.magfilter = GL_NEAREST
	},
	{
		.name = "rnd1n",
		.w = 2, 
		.h = 4,
		.texture_components = text_comps,
		.texture_format = text_form,
		.text_name = GEOMETRIC,
		.g_type = 7,
		.g_seed = 14,
		.continuous = 0,
		.scale = 1.f/4.f,
		.featurescale = 1.f/4.f,
		.minfilter = GL_NEAREST,
		.magfilter = GL_NEAREST
	},
	{
		.name = "rnd2n",
		.w = 4, 
		.h = 4,
		.texture_components = text_comps,
		.texture_format = text_form,
		.text_name = GEOMETRIC,
		.g_type = 7,
		.g_seed = 15,
		.continuous = 0,
		.scale = 1.f/4.f,
		.featurescale = 1.f/4.f,
		.minfilter = GL_NEAREST,
		.magfilter = GL_NEAREST
	}
};


int gfx_opengl_paper_texture_count() { 
	return sizeof(texts)/sizeof(struct texture);
}

GLuint gfx_opengl_paper_texture_get(int i) {
	if (i < 0 || i >= sizeof(texts)/sizeof(struct texture))
		return 0;
	return texts[i].tex;
}

float gfx_opengl_paper_texture_scale_get(int i) {
	if (i < 0 || i >= sizeof(texts)/sizeof(struct texture))
		return 1;
	return texts[i].scale;
}

void gfx_opengl_paper_create_textures()
{
	int i;
	gfx_opengl_check_error("check begin");
	for (i=0; i<(sizeof(texts)/sizeof(struct texture)); i++) {
		struct texture *t = &texts[i];
		float data[t->w * t->h * t->texture_components]; 
		char file[256];
		snprintf(file, sizeof(file), "/tmp/libvob-tex-%d", i);
		printf("file nro: %s\n", file);
		struct stat nonnull;
		if (stat(file, &nonnull) < 0) {
			switch(t->text_name) {
			case FNOISE:
				gfx_opengl_generate_texture_fnoise(t, data);
				break;
			case GEOMETRIC:
				gfx_opengl_generate_texture_geometric(t, data);
				break;
			}
			FILE *f = fopen(file, "w");
			if (f != NULL) {
				fwrite((const void *)data, sizeof(data), 1, f);
				fclose(f);
			}
		} else {
			FILE *f = fopen(file, "r");
			if (f != NULL) {
				fread((void *)data, sizeof(data), 1, f);
				fclose(f);
			}	
		}

		glGenTextures(1, &t->tex);
		glBindTexture(GL_TEXTURE_2D, t->tex);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
				t->magfilter > 0? t->magfilter: GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
				t->minfilter > 0? t->minfilter: GL_LINEAR_MIPMAP_LINEAR);
		//if (self.maxaniso and (self.minfilter + self.magfilter).find("NEAREST")):
                //   self.texture.setTexParameter(target, "TEXTURE_MAX_ANISOTROPY_EXT", self.maxaniso)

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->w, t->h, 0, GL_RGBA, GL_FLOAT, 
			     data);
		printf("texture nro: %d\n", i);
		gfx_opengl_check_error("texture created");
	}
}
