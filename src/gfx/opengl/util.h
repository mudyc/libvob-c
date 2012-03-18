#ifndef GFX_OPENGL_UTIL_H
#define GFX_OPENGL_UTIL_H

#include <GL/glu.h>

#define glError() { \
	GLenum err = glGetError(); \
	while (err != GL_NO_ERROR) { \
		fprintf(stdout, "glError: %s caught at %s:%u\n", (char *)gluErrorString(err), __FILE__, __LINE__); \
		err = glGetError(); \
	} \
}



#define gfx_opengl_check_error(x)		\
	do { \
	int _flag = glGetError(); \
	if (_flag != GL_NO_ERROR) \
		printf("ERROR: %s:%s:%d %s OpenGL: %s\n", \
		       __FILE__, __FUNCTION__, __LINE__, x, gluErrorString(_flag)); \
	} while(0)

#endif
