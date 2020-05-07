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
	hotbarPosition = 0;

	dialog = new Dialogue();

	dial = new DialogStack(dialog);
	prevEngage = false;
	txt = new CombatText();
}

World::~World()
{
}

int countS = 0;

void World::update()
{
	chunkgenUpdate();
	animUpdate();
	playerUpdate();
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

void World::playerUpdate()
{
	hud->setGold(player.gold);
	hud->setEnergy(player.energy);
	hud->setHealth(player.health);
	hud->setHunger(player.hunger);

	if (player.energy < 10 && player.hunger > 2) {
		player.hunger -= 0.001f / 2.0f;
		player.energy += 0.100f / 2.0f;
	}

	if (player.energy >= 10) {
		player.energy = 10;
	}

	if (player.health <= 0) {
		die();
	}


	if (player.health < 20 && player.hunger > 6) {
		player.hunger -= 0.025f / 20.0f;
		player.health += 0.125f / 20.0f;
	}

	if (player.health > 20.5f) {
		player.health = 20.5f;
	}
	if (player.hunger > 20.5f) {
		player.hunger = 20.5f;
	}

	if (player.hunger <= 1) {
		player.health -= 0.1f / 10.0f;
		player.hunger = 0;
	}

	if (player.energy <= 3) {
		controller->speed = 64.0f;
	}
	else {
		controller->speed = 128.0f;
	}
	bool removeEnergy = false;
	int removeAmount = 0;
	g_Inventory->update();
	if (!dialog->isEngaged() && !g_Inventory->isEngaged()) {
		if (Utilities::KeyPressed(PSP_CTRL_LTRIGGER)) {
			hotbarPosition--;
		}
		if (Utilities::KeyPressed(PSP_CTRL_RTRIGGER)) {
			hotbarPosition++;
		}

		if (hotbarPosition > 9) {
			hotbarPosition = 0;
		}

		if (hotbarPosition < 0) {
			hotbarPosition = 9;
		}

		if (Utilities::KeyPressed(PSP_CTRL_TRIANGLE)) {
			g_Inventory->show();
		}

		g_Inventory->setHotbarSelect(hotbarPosition);

		if (Utilities::KeyPressed(PSP_CTRL_CROSS)) {

			if (player.energy >= 1) {
				hud->triggerHit(controller->getCharacterSprite()->getFacing());
				removeEnergy = true;
				removeAmount = 1;

				glm::vec2 pos = controller->getCharacterSprite()->getPosition();

				switch (controller->getCharacterSprite()->getFacing()) {
				case CHARACTER_FACING_DOWN: {
					pos.y += 20;
					break;
				}
				case CHARACTER_FACING_UP: {
					pos.y -= 20;
					break;
				}
				case CHARACTER_FACING_LEFT: {
					pos.x -= 20;
					break;
				}
				case CHARACTER_FACING_RIGHT: {
					pos.x += 20;
					break;
				}
				}

				int x = ((int)pos.x) / 16;
				int y = ((int)pos.y) / 16;

				if ((g_Inventory->getItem(hotbarPosition).ID == Items::BREAD.ID || g_Inventory->getItem(hotbarPosition).ID == Items::APPLE.ID) && player.energy > 5) {
					if (player.hunger < 20.5f) {
						player.hunger += 5;
						player.health += 2;
					}
					g_Inventory->getItemSlot(hotbarPosition)->quantity--;
					if (g_Inventory->getItemSlot(hotbarPosition)->quantity == 0) {
						g_Inventory->getItemSlot(hotbarPosition)->item = Items::NONE;
					}
				}

			}
		}
	}

	dialog->update();
	dial->update();

	if ((dialog->isEngaged() || g_Inventory->isEngaged()) != prevEngage) {


		if (dialog->isEngaged() || g_Inventory->isEngaged()) {
			//Kill handlers
			Utilities::clearActionKeyPairs();
		}
		else {
			//Revive handlers
			Utilities::addActionKeyPair("walkUp", PSP_CTRL_UP);
			Utilities::addActionKeyPair("walkDown", PSP_CTRL_DOWN);
			Utilities::addActionKeyPair("walkLeft", PSP_CTRL_LEFT);
			Utilities::addActionKeyPair("walkRight", PSP_CTRL_RIGHT);
		}

	}
	else {
		if (removeEnergy) {
			player.energy -= removeAmount;
		}
	}
	prevEngage = dialog->isEngaged() || g_Inventory->isEngaged();

	g_Inventory->update();

	txt->update();
	controller->update(0.016f);

	drops->update(controller->getCharacterSprite()->getPosition());
	hud->update();
	hud->setGold(player.gold);
	hud->setEnergy(player.energy);
	hud->setHealth(player.health);
	hud->setHunger(player.hunger);
}

void World::animUpdate()
{

	for (auto [key, chk] : chunkMap) {
		if (countS % 60 == 0) {

			chk->update();
		}
	}
	countS++;
}

void World::chunkgenUpdate()
{
	glm::ivec2 v = { (int)(charSprite->getPosition().x / (32.0f * 8.0f)), (int)(charSprite->getPosition().y / (32.0f * 8.0f)) };

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
				needed.push_back({ x, z, 0 });
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

		controller->clearPhysics();
		for (auto& [pos, chunk] : chunkMap) {
			controller->addPhysicsTileMap(chunk->tmap);
			controller->addPhysicsTileMap(chunk->treemap);
		}

		Utilities::app_Logger->log(std::to_string(controller->getSize()));

		lastPos = v;
	}


}

World* g_World = NULL;