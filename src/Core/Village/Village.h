#pragma once
#include <glm/glm.hpp>
#include "../../World/World.h"

class Village {
public:
	Village(int x, int y);

	void update();
	void draw();

	void dayTrigger();

	void initialSpawn();

private:
	glm::vec2 spawnLocation;
};

extern Village* g_Village;