#include "Village.h"

Village::Village(int x, int y)
{
	spawnLocation = { x, y };
}

void Village::update()
{
	//NPC MANAGER
}

void Village::draw()
{
	//NPC MANAGER
}

void Village::dayTrigger()
{
	//TODO: HANDLE STUFF
}

void Village::initialSpawn()
{
	glm::ivec2 squareTop = { spawnLocation.x - 2, spawnLocation.y - 2 };
	glm::ivec2 squareBot = { spawnLocation.x + 2, spawnLocation.y + 2 };

	for (int x = squareTop.x; x <= squareBot.x; x++) {
		for (int z = squareTop.y; z <= squareBot.y; z++) {
			if (glm::vec2(x, z) != spawnLocation) {
				
				TileAnim* t = new TileAnim();
				t->isAnim = false;
				t->rgba = GU_COLOR((float)g_GameTime.lightLevel / 16.0f, (float)g_GameTime.lightLevel / 16.0f, (float)g_GameTime.lightLevel / 16.0f, 1.0f);
				
				t->layer = -1;
				t->rotation = 0;
				t->physics = false;
				t->indexStart = 0;
				t->tickNumber = 0;
				t->texIndex = 1;
				t->offset = { x * 32, z * 32};
				t->extent = { 32, 32 };

				g_World->setTile(x, z, t);
				
			}
		}
	}
}

Village* g_Village = NULL;