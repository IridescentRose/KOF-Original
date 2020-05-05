#pragma once
#include <Graphics/2D/SpriteBase.h>
#include <Graphics/UI/UIText.h>
#include <Utilities/Logger.h>

using namespace Stardust;
using namespace Stardust::Graphics::Render2D;
using namespace Stardust::Graphics;
using namespace Stardust::Graphics::UI;

class HUD {
public:
	HUD();

	void setHealth(int);
	void setMaxHealth(int);
	void setHunger(int);
	void setEnergy(int);

	void setGold(int);

	void draw();

private:
	Texture* util;

	Sprite* heartF, * heartH, * heartE;
	Sprite* hungerF, * hungerH, * hungerE;
	Sprite* energyF, * energyE;
	Sprite* goldSprite;
	int gold;
	UIText* goldCount;

	int health, hunger, energy;
	int maxHealth;
};