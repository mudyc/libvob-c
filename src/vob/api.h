#ifndef VOB_API_H
#define VOB_API_H

/** @nopython */


// we present the prototypes of Scene and Coordsys here as they are
// part of the main parts. Problems arises if scene.h imports coords.h
// before Scene is declared explicitly.
struct vob_scene;
typedef struct vob_scene Scene;

struct vob_coordinate_system;
typedef struct vob_coordinate_system Coordsys;

enum VOB_TYPE {
	VOB0,
	VOB1,
	VOB2,
};


struct vob {
	enum VOB_TYPE type;

	/** Unique address used in hashing. Also unique as a string to
	 * separate between vobs, e.g. vob.vobs.Rectangle.
	 */
	char *id;
};
typedef struct vob Vob;


/** A visual object that can be rendered without coordinate systems,
 * e.g. back curtain painting.
 */
struct vob0 {
	Vob base;
};
typedef struct vob0 Vob0;


/** A visual object that can be rendered with one coordinate system.
 */
struct vob1 {
	Vob base;
};
typedef struct vob1 Vob1;


/** A visual object that can be rendered with two coordinate systems.
 */
struct vob2 {
	Vob base;
};
typedef struct vob2 Vob2;


#endif
