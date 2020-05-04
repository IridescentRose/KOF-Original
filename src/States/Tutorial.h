#pragma once
#include "../State/GameState.h"
#include "../GameDialog.h"
#include <Graphics/2D/SpriteBase.h>
#include <Utilities/Timer.h>
#include "Menu.h"
#include <Graphics/2D/Tilemap.h>
#include <Graphics/2D/TopDownController.h>
#include "../Core/UI.h"
#include "../Core//Inventory.h"

using namespace Stardust::Graphics;

class TutorialState : public GameState {
public:
	TutorialState();

	void init();
	void cleanup();

	void enter();
	void pause();
	void resume();

	void update(GameStateManager* st);
	void draw(GameStateManager* st);

private:
	Texture* texAtlas, *charTexture;
	TextureAtlas* atlas;
	Render2D::CharacterSprite* charSprite;
	Render2D::TopDownController* controller;
	Render2D::Tilemap* tmap;
	int stage;
	HUD* hud;
	Inventory* inv;
};