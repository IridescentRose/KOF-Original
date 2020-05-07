#include "World.h"
#include <Utilities/Input.h>
#include "../RAM.h"

World::World()
{
	charTexture = TextureUtil::LoadPng("./assets/game/mc.png");
	atlas = new TextureAtlas(16);
	charSprite = new Render2D::CharacterSprite({ 32, 32 }, new TextureAtlas(4), charTexture);

	charSprite->position(240, 136);

	Render2D::CharacterDirectionalAnimInfo* idle = new Render2D::CharacterDirectionalAnimInfo();
	Render2D::CharacterDirectionalAnimInfo* walk = new Render2D::CharacterDirectionalAnimInfo();

	idle->down = { 1, 1 };
	idle->left = { 5, 1 };
	idle->right = { 9, 1 };
	idle->top = { 13,1 };


	walk->down = { 0, 4 };
	walk->left = { 4, 4 };
	walk->right = { 8, 4 };
	walk->top = { 12, 4 };


	charSprite->addAnimEvent("idle", idle);
	charSprite->addAnimEvent("walk", walk);
	charSprite->setDefaultAnimEvent("idle");
	charSprite->triggerAnimEvent("idle");
	charSprite->setFacing(Render2D::CHARACTER_FACING_DOWN);

	controller = new Render2D::TopDownController(charSprite, 128.0f);

	controller->registerHandlers();

	Utilities::addActionKeyPair("walkUp", PSP_CTRL_UP);
	Utilities::addActionKeyPair("walkDown", PSP_CTRL_DOWN);
	Utilities::addActionKeyPair("walkLeft", PSP_CTRL_LEFT);
	Utilities::addActionKeyPair("walkRight", PSP_CTRL_RIGHT);

	controller->getAnimController()->setCharacterTickRateRelative(24);
	controller->setPosition({ 0, 0 });

	player.energy = 10;
	player.health = 20.5f;
	player.hunger = 20.5f;

	hud = new HUD();
	drops = new DropManager();
	lastPos = { -1, -1 };

	debugRAM = new UIText({ 0, 10 }, "RAM: ");
	debugRAM->setOptions({ 0.5f, 0xFFFFFFFF, INTRAFONT_ALIGN_LEFT });

}

World::~World()
{
}

void World::update()
{
	glm::ivec2 v = { (int)(charSprite->getPosition().x / (32.0f*8.0f) ), (int)(charSprite->getPosition().y / (32.0f * 8.0f)) };
	
	if (v != lastPos) {
		//WORLD MANAGEMENT
		glm::vec2 topLeft = { v.x - 2, v.y - 2 };
		glm::vec2 botRight = { v.x + 2, v.y + 2 };
	
		std::vector <Vector3i> needed;
		needed.clear();
		std::vector<Vector3i> excess;
		excess.clear();
		
		for (int x = topLeft.x; x <= botRight.x; x++) {
			for (int z = topLeft.y; z <= botRight.y; z++) {
				needed.push_back({ x, z, 0});
			}
		}
		
		for (auto& [pos, chunk] : chunkMap) {
			bool need = false;
			for (auto& v : needed) {
				if (v == pos) {
					//Is needed
					need = true;
				}
			}
	
			if (!need) {
				excess.push_back(pos);
			}
		}
	
		//DIE OLD ONES!
		for (auto chk : excess) {
			delete chunkMap[chk];
			chunkMap.erase(chk);
		}
	
		//Make new
		for (auto chk : needed) {
			if (chunkMap.find(chk) == chunkMap.end()) {
				//NOT FOUND
				Chunk* chunk = new Chunk(chk.x, chk.y);
				chunk->generate();
				chunk->saveGenerate();
	
				chunkMap.emplace(chk, std::move(chunk));
			}
		}
	
		lastPos = v;
	}
	
	for (auto [key, chk] : chunkMap) {
		chk->update();
	}
	
	hud->setGold(player.gold);
	hud->setEnergy(player.energy);
	hud->setHealth(player.health);
	hud->setHunger(player.hunger);
	
	g_Inventory->update();
	controller->update(0.016f);
	hud->update();
	drops->update(controller->getCharacterSprite()->getPosition());
}

void World::draw()
{
	controller->draw();
	
	for (auto [key, chk] : chunkMap) {
		chk->draw();
	}
	
	drops->draw();
	controller->draw();
	
	g_RenderCore.Set2DMode();
	
	//UI
	hud->draw();
	g_Inventory->drawHotbar();
	g_Inventory->draw();


	//DEBUG
	u32 ramFree = freeMemory();
	debugRAM->setContent("RAM: " + std::to_string(ramFree));
	debugRAM->draw();
}

World* g_World = NULL;