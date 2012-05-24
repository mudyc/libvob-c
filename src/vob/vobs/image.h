#ifndef VOB_VOBS_IMAGE
#define VOB_VOBS_IMAGE


#include "vob/api.h"
#include "vob/scene.h"

/** @nopython
 */
struct _vob_image {
	Vob base;
	char *file;
};
typedef struct _vob_image Image;

Vob1 *vob_image(Scene *vs, char *file);


#endif

