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
	controller->setPosition({ 48 + 50 * 16 * 32, 48 + 50 * 16 * 32 });


	g_Inventory->getItemSlot(0)->item = Items::IRON_SWORD;
	g_Inventory->getItemSlot(0)->quantity = 1;
	g_Inventory->getItemSlot(1)->item = Items::IRON_PICKAXE;
	g_Inventory->getItemSlot(1)->quantity = 1;
	g_Inventory->getItemSlot(2)->item = Items::IRON_AXE;
	g_Inventory->getItemSlot(2)->quantity = 1;
	g_Inventory->getItemSlot(3)->item = Items::IRON_SHOVEL;
	g_Inventory->getItemSlot(3)->quantity = 1;
	g_Inventory->getItemSlot(4)->item = Items::IRON_HOE;
	g_Inventory->getItemSlot(4)->quantity = 1;
	g_Inventory->getItemSlot(5)->item = Items::BREAD;
	g_Inventory->getItemSlot(5)->quantity = 5;



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

TileAnim* World::getTile(int x, int y)
{
	int chkX = x / 16;
	int chkY = y / 16;

	if (chunkMap.find({ chkX, chkY }) != chunkMap.end()) {

		int rX = x % 16;
		int rY = y % 16;

		if (x < 0) {
			rX = -rX;
		}
		if (y < 0) {
			rY = -rY;
		}

		Utilities::app_Logger->log("CHK: " + std::to_string(chkX) + " " + std::to_string(chkY));
		Utilities::app_Logger->log("COORD: " + std::to_string(rX) + " " + std::to_string(rY));

		return chunkMap[{chkX, chkY}]->tmap->getTile(rX + rY * 16);
	}

	return NULL;
}

void World::setTile(int x, int y, TileAnim* t)
{
	int chkX = x / 16;
	int chkY = y / 16;

	if (chunkMap.find({ chkX, chkY }) != chunkMap.end()) {

		int rX = x % 16;
		int rY = y % 16;

		if (x < 0) {
			rX += 16;
		}
		if (y < 0) {
			rY += 16;
		}

		TileAnim* t2 = getTile(x, y);
		t->offset = t2->offset;
		t->extent = t2->extent;

		chunkMap[{chkX, chkY}]->tmap->updateTile(rX + rY * 16, t);
		chunkMap[{chkX, chkY}]->tmap->buildMap();
	}

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
					pos.y += 16;
					break;
				}
				case CHARACTER_FACING_UP: {
					pos.y -= 16;
					break;
				}
				case CHARACTER_FACING_LEFT: {
					pos.x -= 16;
					break;
				}
				case CHARACTER_FACING_RIGHT: {
					pos.x += 16;
					break;
				}
				}

				int x = ((int)pos.x) / 16;
				int y = ((int)pos.y) / 16;

				leftClickInteract(x, y, pos, &removeAmount);

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
		glm::vec2 topLeft = { v.x-2, v.y-2};
		glm::vec2 botRight = { v.x+1, v.y+1};

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

void World::leftClickInteract(int x, int y, glm::vec2 position, int* removeAmount)
{
	if ((g_Inventory->getItem(hotbarPosition).ID == Items::BREAD.ID || g_Inventory->getItem(hotbarPosition).ID == Items::APPLE.ID) && player.energy > 5) {
		if (player.hunger < 20.5f) {
			player.hunger += 5;
			player.health += 2;
		}
		g_Inventory->getItemSlot(hotbarPosition)->quantity--;
		if (g_Inventory->getItemSlot(hotbarPosition)->quantity == 0) {
			g_Inventory->getItemSlot(hotbarPosition)->item = Items::NONE;
		}
		return;
	}

	Tile* hitTile = getTile(x, y);

	if (hitTile != NULL) {

		Utilities::app_Logger->log("TILE HIT: " + std::to_string(x) + " " + std::to_string(y));
		Utilities::app_Logger->log("PLAYER: " + std::to_string(charSprite->getPosition().x) + " " + std::to_string(charSprite->getPosition().y));
		Utilities::app_Logger->log("TILE: " + std::to_string(hitTile->texIndex));

		if (hitTile->texIndex == 27 && g_Inventory->getItem(hotbarPosition).ID == Items::IRON_PICKAXE.ID && player.energy > 5) {
			TileAnim* t = new TileAnim();
			t->layer = 0;
			t->rgba = 0xFFFFFFFF;
			t->rotation = 0;
			t->physics = false;
			t->texIndex = 0;
			setTile(x, y, t);

			(*removeAmount) = 5;
			CombatTextDetails* dt = new CombatTextDetails();
			dt->text = std::to_string(8 + rand() % 5);
			dt->color = 0xFF0000FF;
			dt->ticks = 20;
			dt->pos = { 240, 136 };
			txt->addText(dt);

			ItemDrop* drp = new ItemDrop();
			drp->itm = Items::STONES;
			drp->quantity = 1 + rand() % 4;
			drp->pos = { x * 32 + rand() % 10 , y * 32 + rand() % 10 };

			progInfo.canCompleteMiner = true;

			drops->addDrop(drp);
		}
	}
}

World* g_World = NULL;