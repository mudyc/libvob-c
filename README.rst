
==========================
Library for visual objects
==========================


:author: Matti J. Katila


Libvob is a library for layouting things on a screen like the TeX does. It
also provides nice way to show animations on screen. It should be able
to replace gtk+ and enlightenment =D Yeah, just kidding.

This is my hobby to learn things etc. For example I'm learning practical C 
at the same time. I hope this implementation will end up being small and
efficient to suit well for embedded stuff as well.

Historically this is derivation from libvob (implemented in C++) but
started from scratch. 


Modules in libvob
------------------

Libvob is divided into three modules: lob, vob and gfx.

* *lob* module provides layoutable objects which have capabilities like
  TeX boxes.

* *vob* module provides visual objects. Those that can render themselvs.

* *gfx* module provides window to draw on and binds to different
  implementations. Different implementations will exist for aalib,
  cairo and opengl. This is rather low-level stuff.

The end user, say Peter the programmer, will be doing mostly lob
stuff. Lobs generate(or render as in code) vobs which render on gfx.

Namespace pollution
-------------------

Module naming follows simply as:

   <module>_<function name> e.g. gfx_create_window(), or
   <module>_<sub module>_<function name> e.g. gfx_cairo_create_window().

Structures are named as follows::

   struct lob_color_ {
   	float r, g, b;
   };
   typedef struct lob_color_ LobColor;

   struct lob_rect_ {
	Lob lob;
	LobColor *color;
   };
   typedef struct lob_rect_ LobRect;

And then we can use the constructor as::

   LobColor *lob_color(Region *reg, float r, float g, float b);

All kind of short names are used with CamelCase, e.g., FTP is Ftp, URI
is Uri etc.

One thing I need to think of is library use of libvob-c. Lob or vob
prefix is enough for those but there is this region struct used
everywhere. Let it be for now I can fix it later (I don't like LRegion
nor VRegion - UtilRegion would fit but it's used extensively thus I
like something short).


Depencies
---------

sudo apt-get install libmesa-dev libglu1-mesa-dev libelfg0-dev libpango1.0-dev
