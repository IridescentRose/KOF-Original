#include "Item.h"

Item Items::NONE = { -1, false, false, 0 };
Item Items::IRON_SWORD = { 0, false, true, ITEM_SPECIAL_SWORD };
Item Items::IRON_PICKAXE = { 1, false, true, ITEM_SPECIAL_PICKAXE };
Item Items::IRON_HOE = { 2, false, true, ITEM_SPECIAL_HOE };
Item Items::IRON_AXE = { 3, false, true, ITEM_SPECIAL_AXE };
Item Items::IRON_SHOVEL = { 4, false, true, ITEM_SPECIAL_SHOVEL };
Item Items::WHEAT = { 5, false, false, 0 };
Item Items::BREAD = { 6, false, false, 0 };
Item Items::APPLE = { 7, false, false, 0 };
Item Items::STONES = { 8, false, false, 0 };
Item Items::COAL = { 9, false, false, 0 };
Item Items::IRON_ORE = { 10, false, false, 0 };
Item Items::GOLD_ORE = { 11, false, false, 0 };
Item Items::RUBY_ORE = { 12, false, false, 0 };
Item Items::AMETHYST_ORE = { 13, false, false, 0 };
Item Items::DIAMOND_ORE = { 14, false, false, 0 };
Item Items::GOLDEN_APPLE = { 15, false, false, 0 };
Item Items::LOGS = { 16, false, false, 0 };
Item Items::WOOD = { 17, false, false, 0 };
Item Items::COBBLESTONE = { 18, false, false, 0 };
Item Items::ACORN = { 19, false, false, 0 };

ItemSprites::ItemSprites()
{
	items = TextureUtil::LoadPng("./assets/game/items.png");
	for (int i = 0; i < 20; i++) {
		int x = i % 8;
		int y = i / 8;
		arr[i] = new Sprite(items, x * 16, y * 16, 16, 16, true);
		arr[i]->Scale(1.0f, 1.0f);
	}
}

Sprite* ItemSprites::getSprite(int idx)
{
	if (idx < 20) {
		return arr[idx];
	}
	return NULL;
}
