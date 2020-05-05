#pragma once
#include "../NPC.h"

class NumptyTutorial : public NPC {
public:
	NumptyTutorial(glm::vec2 startPosition, char radius, Tilemap* tmap1, Tilemap* tmap2, std::string file = "./assets/game/NPC/numpty.png", std::string tag = "numpty");

	void simpleAITick();
	void draw();
	void update();
	void handleAction(char action);

	Dialog* getDialog();
	std::string getTag() {
		return tag;
	}
private:
	std::string tag;
};