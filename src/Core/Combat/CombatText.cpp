#include "CombatText.h"

CombatText::CombatText()
{
	txt = new UIText({ 240, 136 }, "");
	txt->setOptions({ 0.5f, 0xFF0000FF, INTRAFONT_ALIGN_CENTER });
	map = NULL;
}

void CombatText::addText(CombatTextDetails* d)
{
	map = d;
}

void CombatText::update()
{
	if (map != NULL) {
		map->pos -= rand() % 4;
		map->ticks--;

		if (map->ticks == 0) {
			delete map;
			map = NULL;
		}
	}
}

void CombatText::draw()
{
	if(map != NULL){
		txt->setContent(map->text);
		txt->setPosition(map->pos);
		txt->setOptions({ 0.5f, (unsigned int)map->color, INTRAFONT_ALIGN_CENTER });

		txt->draw();
	}
}
