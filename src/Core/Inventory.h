#pragma once
#include <Graphics/2D/SpriteBase.h>

using namespace Stardust;
using namespace Stardust::Graphics::Render2D;
using namespace Stardust::Graphics;

class Inventory {
public:
	Inventory();

	void setHotbarSelect(int i);
	void drawHotbar();

private:
	Texture* util;
	Sprite* hotbarSlot;
	Sprite* hotbarSelector;

	int selPos;
};