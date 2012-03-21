#include "lob/composites/button.h"



Lob *lob_button(Region *reg, char *text, LobClickModel *click)
{
	LobButton *ret = REGION(reg, "lob.composite.Button", LobButton);
/*
	ret->base.event = &stack_event;
	ret->base.size = &stack_size;
	ret->base.layout = &stack_layout;
	ret->base.render = &stack_render;

	ret->delegate = lob;*/


	return (Lob*)ret;
}
