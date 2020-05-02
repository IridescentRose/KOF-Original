#pragma once
#include "../State/GameState.h"
#include "../GameDialog.h"
#include <Graphics/UI/UIButton.h>
#include <Audio/AudioClip.h>

using namespace Stardust;
using namespace Stardust::Graphics;

enum MenuStates {
	MENU_MAIN,
	MENU_INSTRUCTION,
	MENU_CREDITS
};

class MenuState : public GameState {
public:
	MenuState();

	void init();
	void cleanup();

	void enter();
	void pause();
	void resume();

	void update(GameStateManager* st);
	void draw(GameStateManager* st);

private:
	UI::UIButton* start;
	UI::UIButton* instr;
	UI::UIButton* credit;
	UI::UIButton* exits;

	Texture* unsel;
	Texture* sel;

	UI::UIText* strt;
	UI::UIText* inst;
	UI::UIText* crdt;
	UI::UIText* exit;


	UI::UIText* instructions;
	UI::UIText* credits;

	Render2D::Sprite2* logo;
	Texture* log;

	GameStateManager* stateMan;
	Audio::AudioClip* mainTheme;
	Audio::AudioClip* click;


	
};