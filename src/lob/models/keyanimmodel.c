#include <stdbool.h>

#include "gfx/gfx_api.h"
#include "gfx/render.h"

#include "util/dbg.h"

#include "lob/models/keyanimmodel.h"

static bool keyanimmodel_tick(LobKeyAnimModel *m, float ms)
{
	//DBG("tick");

	LobAnimModel *ma = (LobAnimModel *)m;
	ma->t += ms / 1000.0f;
	//DBG("ms %f %f %f %d", ms, ma->t, ma->time, (ma->t > ma->time));
	if (ma->t > ma->time) {
		return true;
	}
	float mul = ms;
	m->cs->x += m->dx * mul;
	m->cs->y += m->dy * mul;
	m->cs->z += m->dy * mul;
	m->cs->sx += m->dsx * mul;
	m->cs->sy += m->dsy * mul;
	return false;
}


LobKeyAnimModel *lob_keyanimmodel(Region *reg, float time)
{
	LobKeyAnimModel *ret = REGION(reg, "lob.model.KeyAnim", LobKeyAnimModel);
	LobAnimModel *ret_a = (LobAnimModel *)ret;
	ret_a->time = time;
	ret_a->t = 0;
	ret_a->tick = keyanimmodel_tick;

	ret->inited = false;

	return ret;
}


void lob_keyanimmodel_render(LobKeyAnimModel *m, Lob *that, 
			Coordsys *a, Coordsys *b, 
			float w_, float h_, Scene *vs)
{
	LobAnimModel *ma = (LobAnimModel *)m;
	if (!m->inited) {
		m->inited = true;

		float x,y,z, sx,sy, w,h;
		vob_coords_info(a, &x,&y,&z,&w,&h,&sx,&sy);
		float tx,ty,tz, tsx,tsy, tw,th;
		vob_coords_info(b, &tx,&ty,&tz,&tw,&th,&tsx,&tsy);
		float ms = ma->time * 1000.f;
		m->cs = (OrthoCS *) 
			vob_coords_ortho(vs, b, 
					-(tx - x), -(ty - y), -(tz - z),
					(w*sx)/(tw*tsx), (h*sy)/(th*tsy));
		m->dx = (tx - x) / ms;
		m->dy = (ty - y) / ms;
		m->dz = (tz - z) / ms;
		m->dsx = (tsx - m->cs->sx) / ms;
		m->dsy = (tsy - m->cs->sy) / ms;
		DBG("moving.. %f %f %f %f  %f %f", x, y, sx, sy, w,h );
		DBG("moving.. %f %f %f %f  %f %f", tx, ty, tsx, tsy, tw, th );
		DBG("moving.. %f %f %f %f", m->dx, m->dy, m->dsx, m->dsy );
	} else {
		float mul = gfx_window_instance()->render->t_since_ms;
		m->cs->x += m->dx * mul;
		m->cs->y += m->dy * mul;
		m->cs->z += m->dy * mul;
		m->cs->sx += m->dsx * mul;
		m->cs->sy += m->dsy * mul;
	}
	g_hash_table_insert(vs->anim_set, m, m);
	that->render(that, m->cs, w_, h_, vs);

}
