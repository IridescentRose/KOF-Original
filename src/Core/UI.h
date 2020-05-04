#pragma once
#include <Graphics/2D/SpriteBase.h>

using namespace Stardust;
using namespace Stardust::Graphics::Render2D;
using namespace Stardust::Graphics;

class HUD {
public:
	HUD();

	void setHealth(int);
	void setMaxHealth(int);
	void setHunger(int);
	void setEnergy(int);

	void draw();

private:
	Texture* util;

	Sprite* heartF, * heartH, * heartE;
	Sprite* hungerF, * hungerH, * hungerE;
	Sprite* energyF, * energyE;

	int health, hunger, energy;
	int maxHealth;
};