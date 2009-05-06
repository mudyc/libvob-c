
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

Module naming follows simply as:

   <module>_<function name> as in gfx_create_window(), or
   <module>_<sub module>_<function name> as in gfx_cairo_create_window().

