#include "Inventory.h"

Inventory::Inventory()
{
	util = TextureUtil::LoadPng("./assets/game/utils.png");
	hotbarSlot = new Sprite(util, 0, 8, 8, 8, true);
	hotbarSelector = new Sprite(util, 8, 8, 8, 8, true);
	hotbarSlot->Scale(2.0f, 2.0f);
	hotbarSelector->Scale(2.0f, 2.0f);
	selPos = 0;
}

void Inventory::setHotbarSelect(int i)
{
	selPos = i;
}

void Inventory::drawHotbar()
{
	for (int i = 0; i < 10; i++) {
		hotbarSlot->SetPosition(160 + i * 16, 272 - 18);
		hotbarSlot->Draw();
	}

	hotbarSelector->SetPosition(160 + selPos * 16, 272 - 18);
	hotbarSelector->Draw();
}
