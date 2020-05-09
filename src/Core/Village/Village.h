#pragma once
#include "NPCS/NPCBase.h"
#include <glm/glm.hpp>
#include "Village.h"
#include "../../World/World.h"
#include <vector>
#include "NPCS/NPCFarmer.h"

class Village {
public:
	Village(int x, int y);

	void update();
	void draw();

	void dayTrigger();

	void initialSpawn();

	inline std::vector<NPCBase*> getNPCs() {
		return npcs;
	}

	int farmlandTotal;
	glm::vec2 spawnLocation;
private:


	std::vector<NPCBase*> npcs;
};

extern Village* g_Village;