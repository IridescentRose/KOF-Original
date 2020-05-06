#include "CombatText.h"

CombatText::CombatText()
{
	txt = new UIText({ 240, 136 }, "");
	txt->setOptions({ 0.5f, 0xFF0000FF, INTRAFONT_ALIGN_CENTER });
}

void CombatText::addText(CombatTextDetails* d)
{
	map.emplace(d->color, d);
}

void CombatText::update()
{
	for (auto [color, d] : map) {
		d->pos -= rand() % 4;
		d->ticks--;

		if (d->ticks <= 0) {
			delete d;
			map.erase(color);
		}
	}
}

void CombatText::draw()
{
	for (auto [color, d] : map) {
		txt->setContent(d->text);
		txt->setPosition(d->pos);
		txt->setOptions({ 0.5f, d->color, INTRAFONT_ALIGN_CENTER });

		txt->draw();
	}
}
