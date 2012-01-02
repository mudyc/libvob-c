#ifndef GFX_OPENGL_UTIL_H
#define GFX_OPENGL_UTIL_H

#include <GL/glu.h>

#define gfx_opengl_check_error(x)		\
	do { \
	int _flag = glGetError(); \
	if (_flag != GL_NO_ERROR) \
		printf("ERROR: %s:%s:%d %s OpenGL: %s\n", \
		       __FILE__, __FUNCTION__, __LINE__, x, gluErrorString(_flag)); \
	} while(0)

#endif
