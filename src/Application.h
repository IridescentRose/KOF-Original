#pragma once
#include "State/StateManager.h"

class Application {
public:
	Application();
	~Application();

	void run();

	void update();
	void draw();

	inline bool isRunning() {
		return gsm->isRunning();
	}

private:
	GameStateManager* gsm;
};