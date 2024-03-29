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
#include <Audio/AudioClip.h>

#include "MainState.h"

using namespace Stardust::Graphics;

class DropManager;
struct PlayerInfo {
	float health;
	float hunger;
	float energy;

	int gold;
};

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
	Dialogue* dialog;
	DialogStack* dial;
	bool prevEngage;
	int hotbarPosition;
	CombatText* txt;
	int removeAmount;
	DropManager* drops;
	HUD* hud;
	Audio::AudioClip* clip;
};

extern PlayerInfo player;

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