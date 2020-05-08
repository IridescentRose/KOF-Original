#pragma once
#include "NPCS/NPCBase.h"
#include <glm/glm.hpp>
#include "../../World/World.h"
#include <vector>

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

private:
	glm::vec2 spawnLocation;

	std::vector<NPCBase*> npcs;
};

extern Village* g_Village;