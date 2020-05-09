#pragma once
#include "NPCBase.h"

struct SettlerGuides {
	bool playerHouseSetup;
	bool playerHouseSetupComplete;
	bool playerWorkBenchUse;
	bool playerChestUse;
	bool playerFarmSetup;
	bool playerFarmSetupComplete;
	bool playerNextCharacter;
};

class NPCSettler : public NPCBase{
public:
	NPCSettler(glm::vec2 startPosition, char radius, TilemapAnim* tmap1, TilemapAnim* tmap2, std::string file = "./assets/game/NPC/numpty.png", std::string tag = "numpty");

	Dialog* getDialog();

private:
};

extern SettlerGuides guide;