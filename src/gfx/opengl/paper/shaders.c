/* shaders.c -- GLSL shaders for making papers.
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


#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "gfx/opengl/util.h"

struct shader {
	GLuint prog;
	char *vertex;
	char *frag;
};

static struct shader shaders[] = {
	{
		.vertex = 
		"#version 120\n"
		"uniform vec3 lightDir;\n"
		"varying float intensity;"
		"attribute vec2 InTexCoord0;"
		"attribute vec2 InTexCoord1;"
		"//attribute vec4 gl_Color;\n"
		"//attribute vec4 gl_SecondaryColor;\n"
		"varying vec2 TexCoord0;"
		"varying vec2 TexCoord1;"
		""
		"void main() {"
		"   intensity = dot(lightDir,gl_Normal);"
		"   gl_Position = ftransform();"
		"   TexCoord0 = InTexCoord0;"
		"   TexCoord1 = InTexCoord1;"
		"}"
		,
		.frag = 
		"#version 120\n"
		"uniform sampler2D Texture0;"
		"uniform sampler2D Texture1;"
		"//attribute vec4 gl_Color;\n"
		"//attribute vec4 gl_SecondaryColor;\n"
		"varying vec2 TexCoord0;"
		"varying vec2 TexCoord1;"
		"varying vec4 Color1;"
		"varying vec4 Color2;"
		""
		"vec4 lerp(vec4 a, vec4 b, float fract) {"
		"   return a + fract * (b-a);"
		"}"
		"void main() {"
		"   vec4 texel0 = texture2D(Texture0, TexCoord0);"
		"   vec4 texel1 = texture2D(Texture1, TexCoord1);"
		"   // SPARE0 <- (TEX0 . TEX1)\n"
		"   float spare0 = dot(texel0, texel1);"
		"   float spare1 = spare0 * spare0;"
		"   float ef = spare0 * spare1;"
		"   // lerp(EF, PRI_COL, SEC_COL)\n"
		"   gl_FragColor = lerp(gl_Color, gl_SecondaryColor, ef);"
		"}"
	}
};

static GLuint compile_shader(GLenum type, char *src) 
{
	GLuint shader = glCreateShader(type);

	GLint length = strlen(src);
	glShaderSource(shader, 1, (const char **)&src, &length);
	glCompileShader(shader);
	
	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char *log = malloc(length);
		glGetShaderInfoLog(shader, length, &result, log);

		printf("Compile failed: Unable to compile %s: %s\n", src, log);
		free(log);

		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

GLuint gfx_opengl_paper_shaders_get(int i)
{
	return shaders[i].prog;
}

int gfx_opengl_paper_shaders_count() 
{
	return sizeof(shaders)/sizeof(struct shader);
}

// BUG ATI: This initialization needs to be done before drawing 
// loop is on or deadlock will occure.
void gfx_opengl_paper_shaders_init()
{
	int i;
	GLuint result;

	gfx_opengl_check_error("init with passion");
/*   Maybe you need VAO?  
     See, http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=290649
     * /
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
*/
	for (i=0; i< sizeof(shaders)/sizeof(struct shader); i++) {
		printf("idx: %d\n",i);
		struct shader *s = &shaders[i];
		s->prog = glCreateProgram();

		GLuint shader = compile_shader(GL_VERTEX_SHADER, s->vertex);
		glAttachShader(s->prog, shader);
		glDeleteShader(shader);
		gfx_opengl_check_error("vertex");

		shader = compile_shader(GL_FRAGMENT_SHADER, s->frag);
		glAttachShader(s->prog, shader);
		glDeleteShader(shader);
		gfx_opengl_check_error("fragment");

		printf("link shader\n");
		glLinkProgram(s->prog);
		glGetProgramiv(s->prog, GL_LINK_STATUS, &result);
		if(result == GL_FALSE) {
			GLint length;
			char *log;
			
			glGetProgramiv(s->prog, GL_INFO_LOG_LENGTH, &length);
			log = malloc(length);
			glGetProgramInfoLog(s->prog, length, &result, log);
			
			printf("Shader program(%d) linking failed: %s\n", i, log);
			free(log);
			
			glDeleteProgram(s->prog);
			s->prog = 0;
		}
		gfx_opengl_check_error("just check2");
	}

//	glDeleteVertexArrays(1, &vao);
}
