#pragma once
#include <Graphics/2D/SpriteBase.h>
#include "Item.h"
#include <Graphics/UI/UIText.h>
#include <Utilities/Logger.h>

using namespace Stardust;
using namespace Stardust::Graphics::Render2D;
using namespace Stardust::Graphics;
using namespace Stardust::Graphics::UI;


class Item;
class Items;
class ItemSprites;
class ItemDrop;
class DropManager;

class ItemSlot {
public:
	Item item;
	char quantity;
};

class Inventory {
public:
	Inventory();

	void setHotbarSelect(int i);
	void drawHotbar();

	void show();
	void hide();

	bool tryAddItem(Item itm);
	int findItem(Item itm);

	Item getItem(int slot);

	ItemSlot* getItemSlot(int slot);

	bool isEngaged();

	void update();
	ItemSprites* sprites;
	void draw();

private:
	Texture* util, *inv;
	Sprite* hotbarSlot;
	Sprite* hotbarSelector;
	Sprite* arrow;
	Sprite* inventory;
	UIText* txt;

	bool engaged;

	glm::ivec2 position;

	ItemSlot slots[40];

	ItemSlot held;

	int selPos;
};

extern Inventory* g_Inventory;