#pragma once
#include "../State/GameState.h"
#include "../GameDialog.h"
#include <Graphics/2D/SpriteBase.h>
#include <Utilities/Timer.h>
#include "Menu.h"
#include <Graphics/2D/Tilemap.h>
#include <Graphics/2D/TopDownController.h>
#include "../Core/UI.h"
#include "../Core/Inventory.h"
#include "../Core/PauseHandler.h"
#include "../Core/NPCTutorial/NumptyTutorial.h"
#include "../Core/Dialogue.h"
#include "../Core/Combat/CombatText.h"

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
	std::vector<NPC*> npcs;
	Texture* texAtlas, *charTexture, *tree;
	TextureAtlas* atlas;
	Render2D::CharacterSprite* charSprite;
	Render2D::TopDownController* controller;
	Render2D::Tilemap* tmap;
	Render2D::Tilemap* treemap;
	int stage;
	HUD* hud;
	Inventory* inv;
	Dialogue* dialog;
	DialogStack* dial;
	bool prevEngage;
	int hotbarPosition;
	CombatText* txt;
};

struct TutProgInfo {
	bool talkToSettler;

	bool talkFarmer;
	bool talkMiner;
	bool talkGuard;
	bool talkLumber;

	bool canCompleteFarmer;
	bool canCompleteMiner;
	bool canCompleteGuard;
	bool canCompleteLumber;

	bool completeFarmer;
	bool completeMiner;
	bool completeGuard;
	bool completeLumber;

	bool tutorialTypesTriggered;
	bool tutorialFarmTriggered;
	bool tutorialMineTriggered;
	bool tutorialCombatTriggered;
	bool tutorialLumberTriggered;

	bool tutorialCompleted;
};

extern TutProgInfo progInfo;