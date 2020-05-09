#pragma once
#include "NPCBase.h"

class NPCMiner : public NPCBase {
public:
	NPCMiner(glm::vec2 startPosition, char radius, TilemapAnim* tmap1, TilemapAnim* tmap2, std::string file = "./assets/game/NPC/numpty.png", std::string tag = "numpty");

	Dialog* getDialog();

private:
};