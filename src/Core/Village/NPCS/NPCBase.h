#pragma once
#include "../../NPC.h"

class NPCBase : public NPC {
public:
	NPCBase(glm::vec2 startPosition, char radius, TilemapAnim* tmap1, TilemapAnim* tmap2, std::string file = "./assets/game/NPC/numpty.png", std::string tag = "numpty");

	void simpleAITick();
	void draw();
	void update();
	void handleAction(char action);

	Dialog* getDialog();
	std::string tag;
	bool movedIn;
	bool met;
};