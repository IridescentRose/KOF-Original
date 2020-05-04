#include "UI.h"

HUD::HUD()
{
	health = 20;
	maxHealth = 20;
	hunger = 15;
	energy = 10;

	util = TextureUtil::LoadPng("./assets/game/utils.png");

	heartE = new Sprite(util, 0, 0, 8, 8, true);
	heartF = new Sprite(util, 16, 0, 8, 8, true);
	heartH = new Sprite(util, 8, 0, 8, 8, true);

	heartE->Scale(2.0f, 2.0f);
	heartF->Scale(2.0f, 2.0f);
	heartH->Scale(2.0f, 2.0f);


	energyE = new Sprite(util, 24, 0, 8, 8, true);
	energyF = new Sprite(util, 32, 0, 8, 8, true);

	energyE->Scale(2.0f, 2.0f);
	energyF->Scale(2.0f, 2.0f);

	hungerE = new Sprite(util, 40, 0, 8, 8, true);
	hungerF = new Sprite(util, 56, 0, 8, 8, true);
	hungerH = new Sprite(util, 48, 0, 8, 8, true);

	hungerE->Scale(2.0f, 2.0f);
	hungerF->Scale(2.0f, 2.0f);
	hungerH->Scale(2.0f, 2.0f);
}

void HUD::setHealth(int i)
{
	health = i;
}

void HUD::setMaxHealth(int i)
{
	maxHealth = i;
}

void HUD::setHunger(int i)
{
	hunger = i;
}

void HUD::setEnergy(int i)
{
	energy = i;
}

void HUD::draw()
{
	for (int i = 0; i < maxHealth / 2; i++) {
		heartE->SetPosition(8 + 16 * (i % 10), 8 + 16 * (i / 10));
		heartE->Draw();
	}

	for (int i = 0; i < health / 2; i++) {
		heartF->SetPosition(8 + 16 * (i % 10), 8 + 16 * (i / 10));
		heartF->Draw();
	}

	if (health % 2 != 0) {
		int x = (health % 20) / 2;
		int y = health / 20;

		heartH->SetPosition(8 + 16 * x, 8 + 16 * y);
		heartH->Draw();
	}

	for (int i = 0; i < 10; i++) {
		hungerE->SetPosition((480-168) + 16 * (i), 8);
		hungerE->Draw();
	}

	for (int i = 0; i < hunger / 2; i++) {
		hungerF->SetPosition((480 - 168) + 16 * (i % 10), 8);
		hungerF->Draw();
	}

	if (hunger % 2 != 0) {
		int x = hunger / 2;
		hungerH->SetPosition((480 - 168) + 16 * x, 8);
		hungerH->Draw();
	}
	
	for (int i = 0; i < 10; i++) {
		energyE->SetPosition((480 - 168) + 16 * (i), 24);
		energyE->Draw();
	}

	for (int i = 0; i < energy; i++) {
		energyF->SetPosition((480 - 168) + 16 * (i % 10), 24);
		energyF->Draw();
	}
}
