#pragma once
#include <Graphics/2D/SpriteBase.h>
#include "Item.h"
#include <Graphics/UI/UIText.h>
#include <Utilities/Logger.h>

using namespace Stardust;
using namespace Stardust::Graphics::Render2D;
using namespace Stardust::Graphics;
using namespace Stardust::Graphics::UI;

struct ItemSlot {
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


	bool isEngaged();

	void update();
	void draw();

private:
	Texture* util, *inv;
	Sprite* hotbarSlot;
	Sprite* hotbarSelector;
	Sprite* arrow;
	ItemSprites* sprites;
	Sprite* inventory;
	UIText* txt;

	bool engaged;

	glm::ivec2 position;

	ItemSlot slots[40];

	ItemSlot held;

	int selPos;
};

extern Inventory* g_Inventory;