#ifndef GFX_OPENGL_RENDER_H
#define GFX_OPENGL_RENDER_H

void gfx_opengl_single_render(struct gfx_window *w, Scene *vs);

void gfx_opengl_frame_render(struct gfx_window *w, Scene *vs, Scene *vs2, float fract);



#endif
