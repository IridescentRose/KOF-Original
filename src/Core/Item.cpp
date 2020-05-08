#include "Item.h"
#include "Inventory.h"

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
Item Items::SEEDS = { 20, false, false, 0 };
Item Items::VILLAGESPAWN = { 21, false, false, 0 };
Item Items::WORKBENCH = { 22, false, false, 0 };
Item Items::CHEST = { 23, false, false, 0 };
Item Items::BED = { 24, false, false, 0 };

ItemSprites::ItemSprites()
{
	items = TextureUtil::LoadPng("./assets/game/items.png");
	for (int i = 0; i < 25; i++) {
		int x = i % 8;
		int y = i / 8;
		arr[i] = new Sprite(items, x * 16, y * 16, 16, 16, true);
		arr[i]->Scale(1.0f, 1.0f);
	}
}

Sprite* ItemSprites::getSprite(int idx)
{
	if (idx < 25) {
		return arr[idx];
	}
	return NULL;
}

DropManager::DropManager()
{
	drops.clear();
}

void DropManager::addDrop(ItemDrop* drop)
{
	drops.push_back(drop);
}

void DropManager::clearDrops()
{
	drops.clear();
}

void DropManager::update(glm::vec2 position)
{
	position *= 2.0f;
	std::vector<ItemDrop*> temps;
	temps.clear();

	for (auto a : drops) {
		
		if (((a->pos.x - position.x) * (a->pos.x - position.x)) < (12 * 12) && ((a->pos.y - position.y) * (a->pos.y - position.y)) < (12 * 12)) {
			
			if (g_Inventory->tryAddItem(a->itm)) {
				for (int i = 0; i < a->quantity - 1; i++) {
					g_Inventory->tryAddItem(a->itm);
				}
			}
			else {
				temps.push_back(a);
			}
		}
		else {
			temps.push_back(a);
		}
	}

	drops.clear();

	for (auto a : temps) {
		drops.push_back(a);
	}
		
}


#include "../States/MainState.h"
void DropManager::draw()
{
	for (auto a : drops) {
		if (a != NULL) {
			auto spr = g_Inventory->sprites->getSprite(a->itm.ID);

			spr->SetPosition(a->pos.x, a->pos.y);
			spr->RGB((float)g_GameTime.lightLevel / 16.0f * 255.0f, (float)g_GameTime.lightLevel / 16.0f * 255.0f, (float)g_GameTime.lightLevel / 16.0f * 255.0f);
			spr->Draw();
			spr->RGB(255, 255, 255);
		}
	}
}
