

#include "lob/api.h"
#include "lob/components/paper.h"
#include "vob/vobs.h"
#include "vob/coords.h"

static Lob *paper_layout(Lob *this, float w, float h)
{
	return this;
}

static void paper_render(Lob *this, Coordsys *into,
			float w, float h, Scene *vs)
{
	LobPaper *p = (LobPaper *) this;
	Vob1 *vob = vob_paper(vs, p->id);

	Coordsys *cs = vob_coords_box(vs, into, 0,0,w, h);

	vob_scene_put1(vs, vob, cs);
}

LobPaper *lob_paper(Region *reg, unsigned long id)
{
	LobPaper *ret = REGION(reg, "lob.component.Paper", LobPaper);

	ret->lob.reg = reg;
	ret->lob.size = &lob_size_default;
	ret->lob.layout = &paper_layout;
	ret->lob.render = &paper_render;

	ret->id = id;
	return ret;
}
