#include "Village.h"
#include "NPCS/NPCSettler.h"
#include "NPCS/NPCFarmer.h"
#include "NPCS/NPCMiner.h"
#include "NPCS/NPCLumberJack.h"

Village::Village(int x, int y)
{
	spawnLocation = { x, y };
	farmlandTotal = 0;
}

void Village::update()
{
	for (auto npc : npcs) {
		npc->simpleAITick();
		npc->update();
	}
}

void Village::draw()
{
	for (auto npc : npcs) {
		npc->draw();
	}
}

void Village::dayTrigger()
{
	if (g_GameTime.days == 1) {
		npcs.push_back(new NPCFarmer(spawnLocation * 32.0f  + glm::vec2(24, 24), 3, g_World->chunkMap[{(int)spawnLocation.x / 16, (int)spawnLocation.y / 16, 0}]->tmap, g_World->chunkMap[{(int)spawnLocation.x / 16, (int)spawnLocation.y / 16, 0}]->treemap, "./assets/game/NPC/farmer.png", "farmer"));
	}
	else {
		int randomNPC =  rand() % 3;

		switch (randomNPC) {
		case 0: {
			npcs.push_back(new NPCFarmer(spawnLocation * 32.0f + glm::vec2(24, 24), 3, g_World->chunkMap[{(int)spawnLocation.x / 16, (int)spawnLocation.y / 16, 0}]->tmap, g_World->chunkMap[{(int)spawnLocation.x / 16, (int)spawnLocation.y / 16, 0}]->treemap, "./assets/game/NPC/farmer.png", "farmer"));
			break;
		}
		case 1: {
			npcs.push_back(new NPCMiner(spawnLocation * 32.0f + glm::vec2(24, 24), 3, g_World->chunkMap[{(int)spawnLocation.x / 16, (int)spawnLocation.y / 16, 0}]->tmap, g_World->chunkMap[{(int)spawnLocation.x / 16, (int)spawnLocation.y / 16, 0}]->treemap, "./assets/game/NPC/miner.png", "miner"));
			break;
		}
		case 2: {
			npcs.push_back(new NPCLumber(spawnLocation * 32.0f + glm::vec2(24, 24), 3, g_World->chunkMap[{(int)spawnLocation.x / 16, (int)spawnLocation.y / 16, 0}]->tmap, g_World->chunkMap[{(int)spawnLocation.x / 16, (int)spawnLocation.y / 16, 0}]->treemap, "./assets/game/NPC/lumberjack.png", "lumberjack"));
			break;
		}
		}
	}

	guide.playerNextCharacter = true;
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

	npcs.push_back(new NPCSettler(spawnLocation * 32.0f - glm::vec2(24, 24), 3, g_World->chunkMap[{(int)spawnLocation.x / 16, (int)spawnLocation.y / 16, 0}]->tmap, g_World->chunkMap[{(int)spawnLocation.x / 16, (int)spawnLocation.y / 16, 0}]->treemap, "./assets/game/NPC/settler.png", "settler"));
}

Village* g_Village = NULL;