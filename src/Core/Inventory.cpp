#include "Inventory.h"

Inventory::Inventory()
{
	util = TextureUtil::LoadPng("./assets/game/utils.png");
	hotbarSlot = new Sprite(util, 0, 8, 8, 8, true);
	hotbarSelector = new Sprite(util, 8, 8, 8, 8, true);
	hotbarSlot->Scale(4.0f, 4.0f);
	hotbarSelector->Scale(4.0f, 4.0f);
	selPos = 0;
}

void Inventory::setHotbarSelect(int i)
{
	selPos = i;
}

void Inventory::drawHotbar()
{
	for (int i = 0; i < 10; i++) {
		hotbarSlot->SetPosition(80 + i * 32, 272 - 34);
		hotbarSlot->Draw();
	}

	hotbarSelector->SetPosition(80 + selPos * 32, 272 - 34);
	hotbarSelector->Draw();
}
