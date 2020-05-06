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

	goldSprite = new Sprite(util, 24, 8, 8, 8, true);
	goldSprite->Scale(2.0f, 2.0f);
	gold = 0;

	hitU = new Sprite(util, 0, 16, 8, 8, true);
	hitU->Scale(4.0f, 4.0f);
	hitU->SetPosition(240 - 17, 136 - 42);
	hitR = new Sprite(util, 8, 16, 8, 8, true);
	hitR->Scale(4.0f, 4.0f);
	hitR->SetPosition(240 + 8, 136 - 10);
	hitL = new Sprite(util, 16, 16, 8, 8, true);
	hitL->Scale(4.0f, 4.0f);
	hitL->SetPosition(240 - 40, 136 - 10);
	hitD = new Sprite(util, 24, 16, 8, 8, true);
	hitD->Scale(4.0f, 4.0f);
	hitD->SetPosition(240 - 17, 136 + 10);

	goldCount = new UIText({480 - 24, 60}, std::to_string(gold));
	goldCount->setOptions({ 0.5f, 0xFFFFFFFF, INTRAFONT_ALIGN_RIGHT });
	tick = -1;
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

void HUD::setGold(int i)
{
	gold = i;
	goldCount->setContent(std::to_string(gold));
}

void HUD::triggerHit(char dir)
{
	d = dir;
	tick = 6;
}

void HUD::update()
{
	tick--;
}

void HUD::draw()
{
	if (tick > 0) {
		switch (d) {
		case CHARACTER_FACING_DOWN: {
			hitD->Draw();
			break;
		}
		case CHARACTER_FACING_UP: {
			hitU->Draw();
			break;
		}
		case CHARACTER_FACING_LEFT: {
			hitL->Draw();
			break;
		}
		case CHARACTER_FACING_RIGHT: {
			hitR->Draw();
			break;
		}
		}
	}

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

	goldSprite->SetPosition(480 - 24, 48);
	goldSprite->Draw();

	goldCount->draw();
}
