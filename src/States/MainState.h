#pragma once
#include "../GameDialog.h"
#include "../World/World.h"

#include "Menu.h"
#include "Tutorial.h"
#include "../World/World.h"

class MainState : public GameState {
public:
	MainState();

	void init();
	void cleanup();

	void enter();
	void pause();
	void resume();

	void update(GameStateManager* st);
	void draw(GameStateManager* st);

private:
};