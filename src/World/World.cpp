#include "World.h"
#include <Utilities/Input.h>
#include "../RAM.h"
#include "../Core/Village/NPCS/NPCSettler.h"

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

	controller->getAnimController()->setCharacterTickRateRelative(12);
	controller->setPosition({ 48 + 50 * 16 * 32, 48 + 50 * 16 * 32 });

#ifdef KOF_DEBUG

	g_Inventory->getItemSlot(0)->item = Items::IRON_SWORD;
	g_Inventory->getItemSlot(0)->quantity = 1;
	g_Inventory->getItemSlot(1)->item = Items::IRON_PICKAXE;
	g_Inventory->getItemSlot(1)->quantity = 1;
	g_Inventory->getItemSlot(2)->item = Items::IRON_AXE;
	g_Inventory->getItemSlot(2)->quantity = 1;
	g_Inventory->getItemSlot(3)->item = Items::IRON_HOE;
	g_Inventory->getItemSlot(3)->quantity = 1;
	g_Inventory->getItemSlot(4)->item = Items::BREAD;
	g_Inventory->getItemSlot(4)->quantity = 5;

#endif

	g_Inventory->tryAddItem(Items::VILLAGESPAWN);

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
	Dialog* d = new Dialog();
	d->interactionType = INTERACTION_TYPE_NONE;
	d->text = "Settler: Welcome to the wilderness, explorer!\nI know it's been a long journey, but we can finally begin\nto set up a village. I have placed a Village Indicator\nin your inventory. Please place it where you wish for\nthe village to begin. Be wise about your location!";

	Dialog* d2 = new Dialog();
	d2->interactionType = INTERACTION_TYPE_NONE;
	d2->text = "Settler: I will join you again once you have made your\nchoice! Don't worry, I will teleport to your location.\nTo place the indicator, use the X button over grass. This\nwill set up a base camp to expand from.\nGood luck!";

	Dialog* d3 = new Dialog();
	d3->interactionType = INTERACTION_TYPE_NONE;
	d3->text = "Tutorial: In the wilderness you are less confined\nthan the village. You can place blocks by using X in\nsuitable locations like on grass.\nDo be aware that the days will pass quickly!";


	dial = new DialogStack(dialog);
	dial->addDialog(d);
	dial->addDialog(d2);
	dial->addDialog(d3);
	prevEngage = false;
	txt = new CombatText();


	currLevel = 10;
	g_GameTime.lightLevel = currLevel;
	charSprite->setColor(GU_COLOR((float)g_GameTime.lightLevel / 16.0f, (float)g_GameTime.lightLevel / 16.0f, (float)g_GameTime.lightLevel / 16.0f, 1.0f));

	playerRespawnPos = charSprite->getPosition() * 2.0f;
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

	if (g_Village != NULL) {
		g_Village->update();
	}
}

void World::draw()
{

	sceGumMatrixMode(GU_VIEW);
	sceGumLoadIdentity();
	ScePspFVector3 v = { 240 - controller->getCharacterSprite()->getPosition().x * 2, 136 - controller->getCharacterSprite()->getPosition().y * 2, 0.0f };
	sceGumTranslate(&v);
	sceGumMatrixMode(GU_MODEL);
	
	for (auto [key, chk] : chunkMap) {
		chk->draw();
	}
	
	drops->draw();

	if (g_Village != NULL) {
		g_Village->draw();
	}

	controller->draw();

	g_RenderCore.Set2DMode();
	
	//UI
	hud->draw();
	g_Inventory->drawHotbar();
	g_Inventory->draw();
	txt->draw();
	dialog->draw();

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
		die2();
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

	if (countS % 3 == 0) {
		g_GameTime.totalTime += 2;
		g_GameTime.dayTime = g_GameTime.totalTime % 24001;
		g_GameTime.days = g_GameTime.totalTime / 24000;

		if (g_GameTime.days != currDay) {
			currDay = g_GameTime.days;

			if (g_Village != NULL) {
				g_Village->dayTrigger();
			}
		}
	}

	bool lightingUpdate = false;
	//LIGHTING SYSTEM
	if (g_GameTime.dayTime > 0 && g_GameTime.dayTime <= 3000) {
		int newLevel = 10 + g_GameTime.dayTime / 500;
		if (newLevel != currLevel) {
			currLevel = newLevel;
			lightingUpdate = true;
		}
	}
	else if (g_GameTime.dayTime > 3000 && g_GameTime.dayTime <= 9000) {
		currLevel = 16;
	}
	else if (g_GameTime.dayTime > 9000 && g_GameTime.dayTime <= 14000) {
		int newLevel = 16 - (g_GameTime.dayTime - 9000) / 500;
		if (newLevel != currLevel) {
			currLevel = newLevel;
			lightingUpdate = true;
		}
	}
	else if (g_GameTime.dayTime > 14000 && g_GameTime.dayTime < 22000) {
		currLevel = 6;
	}
	else if (g_GameTime.dayTime > 22000 && g_GameTime.dayTime <= 24000) {
		int newLevel = 6 + (g_GameTime.dayTime - 22000) / 500;
		if (newLevel != currLevel) {
			currLevel = newLevel;
			lightingUpdate = true;
		}
	}

	if (lightingUpdate) {
		for (auto [key, chk] : chunkMap) {
			chk->setLighting(currLevel);
		}
		charSprite->setColor(GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f));
	}

	if (g_GameTime.totalTime % 4000 == 0 && countS % 3 == 0) {
		for (auto [key, chk] : chunkMap) {
			chk->updateTiles();
		}
	}

	g_GameTime.lightLevel = currLevel;
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
				chunk->setLighting(currLevel);

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

void World::respawn()
{
	player.health = 20;
	player.energy = 10;
	player.hunger = 20;
	player.gold = 0;

	controller->setPosition(playerRespawnPos);

}

void World::leftClickInteract(int x, int y, glm::vec2 pos, int* removeAmount)
{
	if(g_Village != NULL){
		for (auto npc : g_Village->getNPCs()) {
			glm::vec2 pos = npc->getPosition() / 2.0f;
			glm::vec2 playerPos = controller->getCharacterSprite()->getPosition();

			Utilities::app_Logger->log("PLAYER " + std::to_string(playerPos.x) + " " + std::to_string(playerPos.y));
			Utilities::app_Logger->log("NPC " + std::to_string(pos.x) + " " + std::to_string(pos.x));


			if (((pos.x - playerPos.x) * (pos.x - playerPos.x)) < (16 * 16) && ((pos.y - playerPos.y) * (pos.y - playerPos.y)) < (16 * 16)) {
				//INTERACT
				dial->addDialog(npc->getDialog());
				return;
			}

			pos *= 2.0f;
			if (((pos.x - playerPos.x) * (pos.x - playerPos.x)) < (16 * 16) && ((pos.y - playerPos.y) * (pos.y - playerPos.y)) < (16 * 16)) {
				//INTERACT
				dial->addDialog(npc->getDialog());
				return;
			}


		}
	}

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


		if (hitTile->texIndex == 27 && g_Inventory->getItem(hotbarPosition).ID == Items::IRON_PICKAXE.ID && player.energy > 5) {
			TileAnim* t = new TileAnim();
			t->layer = 0;
			t->rgba = GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f);
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
			drops->addDrop(drp);
			return;
		}

		if (hitTile->texIndex == 16 || hitTile->texIndex == 17 || hitTile->texIndex == 32 || hitTile->texIndex == 33) {
			TileAnim* t = new TileAnim();
			t->layer = 0;
			t->rgba = GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f);
			t->rotation = 0;
			t->physics = false;
			t->texIndex = 0;
			setTile(x, y, t);

			ItemDrop* drp = new ItemDrop();
			drp->itm = Items::SEEDS;
			drp->quantity = 1;
			drp->pos = { x * 32 + rand() % 10 , y * 32 + rand() % 10 };
			drops->addDrop(drp);
		}

		if (hitTile->texIndex == 0 && g_Inventory->getItem(hotbarPosition).ID == Items::IRON_SHOVEL.ID && player.energy > 5) {
			TileAnim* t = new TileAnim();
			t->layer = 0;
			t->rgba = GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f);
			t->rotation = 0;
			t->physics = false;
			t->texIndex = 2;
			setTile(x, y, t);

			(*removeAmount) = 5;
			CombatTextDetails* dt = new CombatTextDetails();
			dt->text = std::to_string(8 + rand() % 5);
			dt->color = 0xFF0000FF;
			dt->ticks = 20;
			dt->pos = { 240, 136 };
			txt->addText(dt);

			ItemDrop* drp = new ItemDrop();
			drp->itm = Items::SEEDS;
			drp->quantity = 1;
			drp->pos = { x * 32 + rand() % 10 , y * 32 + rand() % 10 };
			drops->addDrop(drp);
			return;
		}

		if (hitTile->texIndex == 0 && g_Inventory->getItem(hotbarPosition).ID == Items::ACORN.ID && player.energy > 1) {
			TileAnim* t = new TileAnim();
			t->layer = 0;
			t->rgba = GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f);
			t->rotation = 0;
			t->physics = false;
			t->texIndex = 38;
			t->isAnim = true;
			t->indexStart = 38;
			t->animLength = 3;
			setTile(x, y, t);

			ItemSlot* slt = g_Inventory->getItemSlot(hotbarPosition);
			slt->quantity--;

			if (slt->quantity == 0) {
				slt->item = Items::NONE;
			}
			return;
		}

		if (hitTile->texIndex == 0 && g_Inventory->getItem(hotbarPosition).ID == Items::VILLAGESPAWN.ID && player.energy > 1) {
			TileAnim* t = new TileAnim();
			t->layer = 0;
			t->rgba = GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f);
			t->rotation = 0;
			t->physics = false;
			t->texIndex = 42;
			setTile(x, y, t);

			ItemSlot* slt = g_Inventory->getItemSlot(hotbarPosition);
			slt->quantity--;

			if (slt->quantity == 0) {
				slt->item = Items::NONE;
			}

			g_Village = new Village(x, y);
			g_Village->initialSpawn();

			playerRespawnPos = charSprite->getPosition() * 2.0f;

			return;
		}

		if (hitTile->texIndex == 0 && g_Inventory->getItem(hotbarPosition).ID == Items::WORKBENCH.ID && player.energy > 1) {
			TileAnim* t = new TileAnim();
			t->layer = 0;
			t->rgba = GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f);
			t->rotation = 0;
			t->physics = false;
			t->texIndex = 43;
			setTile(x, y, t);

			ItemSlot* slt = g_Inventory->getItemSlot(hotbarPosition);
			slt->quantity--;

			if (slt->quantity == 0) {
				slt->item = Items::NONE;
			}


			if (!guide.playerWorkBenchUse) {
				guide.playerWorkBenchUse = true;
				Dialog* d = new Dialog();
				d->interactionType = INTERACTION_TYPE_NONE;
				d->text = "Tutorial: This is a workbench. By interacting, you can\ncraft new blocks and items to help your village grow!";
				dial->addDialog(d);
			}

			return;
		}

		if (hitTile->texIndex == 0 && g_Inventory->getItem(hotbarPosition).ID == Items::WOOD.ID && player.energy > 1) {
			TileAnim* t = new TileAnim();
			t->layer = 0;
			t->rgba = GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f);
			t->rotation = 0;
			t->physics = true;
			t->texIndex = 4;
			setTile(x, y, t);

			ItemSlot* slt = g_Inventory->getItemSlot(hotbarPosition);
			slt->quantity--;

			if (slt->quantity == 0) {
				slt->item = Items::NONE;
			}

		}

		if (hitTile->texIndex == 0 && g_Inventory->getItem(hotbarPosition).ID == Items::FARMERTABLE.ID && player.energy > 1) {
			TileAnim* t = new TileAnim();
			t->layer = 0;
			t->rgba = GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f);
			t->rotation = 0;
			t->physics = false;
			t->texIndex = 44;
			setTile(x, y, t);

			ItemSlot* slt = g_Inventory->getItemSlot(hotbarPosition);
			slt->quantity--;

			if (slt->quantity == 0) {
				slt->item = Items::NONE;
			}

			g_Village->getNPCs().back()->controller->setPosition(controller->getCharacterSprite()->getPosition() *= 2.0f);
			g_Village->getNPCs().back()->movedIn = true;


		}

		if (hitTile->texIndex == 0 && g_Inventory->getItem(hotbarPosition).ID == Items::MINERTABLE.ID && player.energy > 1) {
			TileAnim* t = new TileAnim();
			t->layer = 0;
			t->rgba = GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f);
			t->rotation = 0;
			t->physics = false;
			t->texIndex = 45;
			setTile(x, y, t);

			ItemSlot* slt = g_Inventory->getItemSlot(hotbarPosition);
			slt->quantity--;

			if (slt->quantity == 0) {
				slt->item = Items::NONE;
			}

			g_Village->getNPCs().back()->controller->setPosition(controller->getCharacterSprite()->getPosition() *= 2.0f);
			g_Village->getNPCs().back()->movedIn = true;


		}
		if (hitTile->texIndex == 0 && g_Inventory->getItem(hotbarPosition).ID == Items::LUMBERTABLE.ID && player.energy > 1) {
			TileAnim* t = new TileAnim();
			t->layer = 0;
			t->rgba = GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f);
			t->rotation = 0;
			t->physics = false;
			t->texIndex = 46;
			setTile(x, y, t);

			ItemSlot* slt = g_Inventory->getItemSlot(hotbarPosition);
			slt->quantity--;

			if (slt->quantity == 0) {
				slt->item = Items::NONE;
			}

			g_Village->getNPCs().back()->controller->setPosition(controller->getCharacterSprite()->getPosition() *= 2.0f);
			g_Village->getNPCs().back()->movedIn = true;


		}


		if (hitTile->texIndex == 0 && g_Inventory->getItem(hotbarPosition).ID == Items::COBBLESTONE.ID && player.energy > 1) {
			TileAnim* t = new TileAnim();
			t->layer = 0;
			t->rgba = GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f);
			t->rotation = 0;
			t->physics = false;
			t->texIndex = 1;
			setTile(x, y, t);

			ItemSlot* slt = g_Inventory->getItemSlot(hotbarPosition);
			slt->quantity--;

			if (slt->quantity == 0) {
				slt->item = Items::NONE;
			}

		}

		if (hitTile->texIndex == 4 && g_Inventory->getItem(hotbarPosition).ID == Items::IRON_AXE.ID && player.energy > 1) {
			TileAnim* t = new TileAnim();
			t->layer = 0;
			t->rgba = GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f);
			t->rotation = 0;
			t->physics = false;
			t->texIndex = 0;
			setTile(x, y, t);

			ItemDrop* drp = new ItemDrop();
			drp->itm = Items::WOOD;
			drp->quantity = 1;
			drp->pos = { x * 32 + rand() % 10 , y * 32 + rand() % 10 };
			drops->addDrop(drp);

			(*removeAmount) = 5;
			return;
		}

		if (hitTile->texIndex == 1 && g_Inventory->getItem(hotbarPosition).ID == Items::IRON_PICKAXE.ID && player.energy > 1) {
			TileAnim* t = new TileAnim();
			t->layer = 0;
			t->rgba = GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f);
			t->rotation = 0;
			t->physics = false;
			t->texIndex = 0;
			setTile(x, y, t);

			ItemDrop* drp = new ItemDrop();
			drp->itm = Items::COBBLESTONE;
			drp->quantity = 1;
			drp->pos = { x * 32 + rand() % 10 , y * 32 + rand() % 10 };
			drops->addDrop(drp);

			(*removeAmount) = 5;
			return;
		}


		if (hitTile->texIndex == 43 && g_Inventory->getItem(hotbarPosition).ID != Items::IRON_AXE.ID) {

			Dialog* d = new Dialog();
			d->interactionType = INTERACTION_TYPE_TRADE;
			d->text = "What would you like to make?";
			d->trades.clear();

			Trade* t1 = new Trade();
			t1->item1 = Items::LOGS;
			t1->quantity1 = 1;
			t1->item2 = Items::WOOD;
			t1->quantity2 = 2;

			Trade* t2 = new Trade();
			t2->item1 = Items::LOGS;
			t2->quantity1 = 4;
			t2->item2 = Items::WORKBENCH;
			t2->quantity2 = 1;

			Trade* t3 = new Trade();
			t3->item1 = Items::LOGS;
			t3->quantity1 = 8;
			t3->item2 = Items::CHEST;
			t3->quantity2 = 1;

			Trade* t4 = new Trade();
			t4->item1 = Items::STONES;
			t4->quantity1 = 4;
			t4->item2 = Items::COBBLESTONE;
			t4->quantity2 = 1;

			d->trades.clear();
			d->trades.push_back(t1);
			d->trades.push_back(t2);
			d->trades.push_back(t3);
			d->trades.push_back(t4);
			
			dial->addDialog(d);

			return;
		}

		if (hitTile->texIndex == 43 && g_Inventory->getItem(hotbarPosition).ID == Items::IRON_AXE.ID && player.energy > 5) {
			TileAnim* t = new TileAnim();
			t->layer = 0;
			t->rgba = GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f);
			t->rotation = 0;
			t->physics = false;
			t->texIndex = 0;
			setTile(x, y, t);

			ItemDrop* drp = new ItemDrop();
			drp->itm = Items::WORKBENCH;
			drp->quantity = 1;
			drp->pos = { x * 32 + rand() % 10 , y * 32 + rand() % 10 };
			drops->addDrop(drp);

			return;
		}


		if (hitTile->texIndex == 0 && g_Inventory->getItem(hotbarPosition).ID == Items::CHEST.ID && player.energy > 1) {
			TileAnim* t = new TileAnim();
			t->layer = 0;
			t->rgba = GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f);
			t->rotation = 0;
			t->physics = false;
			t->texIndex = 9;
			setTile(x, y, t);

			ItemSlot* slt = g_Inventory->getItemSlot(hotbarPosition);
			slt->quantity--;

			if (slt->quantity == 0) {
				slt->item = Items::NONE;
			}


			if (!guide.playerChestUse) {
				guide.playerChestUse = true;
				Dialog* d = new Dialog();
				d->interactionType = INTERACTION_TYPE_NONE;
				d->text = "Tutorial: This is a chest. By interacting, you can\nplace items inside to store them!";
				dial->addDialog(d);
			}

			return;
		}

		if (hitTile->texIndex == 0 && g_Inventory->getItem(hotbarPosition).ID == Items::BED.ID && player.energy > 1) {
			TileAnim* t = new TileAnim();
			t->layer = 0;
			t->rgba = GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f);
			t->rotation = 0;
			t->physics = false;
			t->texIndex = 12;
			setTile(x, y, t);

			TileAnim* t2 = new TileAnim();
			t2->layer = 0;
			t2->rgba = GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f);
			t2->rotation = 0;
			t2->physics = false;
			t2->texIndex = 13;
			setTile(x+1, y, t2);

			ItemSlot* slt = g_Inventory->getItemSlot(hotbarPosition);
			slt->quantity--;

			if (slt->quantity == 0) {
				slt->item = Items::NONE;
			}

			playerRespawnPos = charSprite->getPosition() * 2.0f;

			if (!guide.playerHouseSetupComplete) {
				Dialog* d = new Dialog();
				d->interactionType = INTERACTION_TYPE_NONE;
				d->text = "Tutorial: This is your bed. It is now your permanent\nspawn point! If you die, you will be brought back here!";
				dial->addDialog(d);
			}

			guide.playerHouseSetupComplete = true;

			return;
		}

		if (hitTile->texIndex == 2 && g_Inventory->getItem(hotbarPosition).ID == Items::SEEDS.ID && player.energy > 5) {
			TileAnim* t = new TileAnim();
			t->layer = 0;
			t->rgba = GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f);
			t->rotation = 0;
			t->physics = false;
			t->texIndex = 0;
			setTile(x, y, t);


			ItemSlot* slt = g_Inventory->getItemSlot(hotbarPosition);
			slt->quantity--;

			if (slt->quantity == 0) {
				slt->item = Items::NONE;
			}

			return;
		}

		if (hitTile->texIndex == 2 && g_Inventory->getItem(hotbarPosition).ID == Items::IRON_HOE.ID && player.energy > 5) {
			TileAnim* t = new TileAnim();
			t->layer = 0;
			t->rgba = GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f);
			t->rotation = 0;
			t->physics = false;
			t->texIndex = 22;
			setTile(x, y, t);

			(*removeAmount) = 5;
			CombatTextDetails* dt = new CombatTextDetails();
			dt->text = std::to_string(8 + rand() % 5);
			dt->color = 0xFF0000FF;
			dt->ticks = 20;
			dt->pos = { 240, 136 };
			txt->addText(dt);

			return;
		}

		if (hitTile->texIndex == 22 && g_Inventory->getItem(hotbarPosition).ID == Items::IRON_HOE.ID && player.energy > 5) {
			TileAnim* t = new TileAnim();
			t->layer = 0;
			t->rgba = GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f);
			t->rotation = 0;
			t->physics = false;
			t->texIndex = 2;
			setTile(x, y, t);

			(*removeAmount) = 5;
			CombatTextDetails* dt = new CombatTextDetails();
			dt->text = std::to_string(8 + rand() % 5);
			dt->color = 0xFF0000FF;
			dt->ticks = 20;
			dt->pos = { 240, 136 };
			txt->addText(dt);

			if (g_Village != NULL) {
				if ((g_Village->spawnLocation.x - x) * (g_Village->spawnLocation.x - x) < (20 * 20) && (g_Village->spawnLocation.y - y) * (g_Village->spawnLocation.y - y) < (20 * 20)) {
					g_Village->farmlandTotal--;
				}
			}

			return;
		}

		if (hitTile->texIndex == 22 && g_Inventory->getItem(hotbarPosition).ID == Items::SEEDS.ID && player.energy > 5) {
			TileAnim* t = new TileAnim();
			t->layer = 0;
			t->rgba = GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f);
			t->rotation = 0;
			t->physics = false;
			t->texIndex = 23;
			setTile(x, y, t);

			ItemSlot* slt = g_Inventory->getItemSlot(hotbarPosition);
			slt->quantity--;

			if (slt->quantity == 0) {
				slt->item = Items::NONE;
			}

			if (g_Village != NULL) {
				if ((g_Village->spawnLocation.x - x) * (g_Village->spawnLocation.x - x) < (20 * 20) && (g_Village->spawnLocation.y - y) * (g_Village->spawnLocation.y - y) < (20 * 20)) {
					g_Village->farmlandTotal++;
					guide.playerFarmSetupComplete = true;
				}
			}

			return;
		}

		if ((hitTile->texIndex == 0 || hitTile->texIndex == 29) && g_Inventory->getItem(hotbarPosition).ID == Items::LOGS.ID && player.energy > 5) {
			TileAnim* t = new TileAnim();
			t->layer = 0;
			t->rgba = GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f);
			t->rotation = 0;
			t->physics = true;
			t->texIndex = 31;
			setTile(x, y, t);

			ItemSlot* slt = g_Inventory->getItemSlot(hotbarPosition);
			slt->quantity--;

			if (slt->quantity == 0) {
				slt->item = Items::NONE;
			}
			return;
		}



		if (hitTile->texIndex == 31 && g_Inventory->getItem(hotbarPosition).ID == Items::IRON_AXE.ID && player.energy > 5) {
			TileAnim* t = new TileAnim();
			t->layer = 0;
			t->rgba = GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f);
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
			drp->itm = Items::LOGS;
			drp->quantity = 1;
			drp->pos = { x * 32 + rand() % 10 , y * 32 + rand() % 10 };
			drops->addDrop(drp);

			return;
		}

		if (hitTile->texIndex >= 23 && hitTile->texIndex <= 25 && g_Inventory->getItem(hotbarPosition).ID == Items::IRON_HOE.ID && player.energy > 5) {
			TileAnim* t = new TileAnim();
			t->layer = 0;
			t->rgba = GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f);
			t->rotation = 0;
			t->physics = false;
			t->texIndex = 22;
			setTile(x, y, t);

			(*removeAmount) = 5;
			CombatTextDetails* dt = new CombatTextDetails();
			dt->text = std::to_string(8 + rand() % 5);
			dt->color = 0xFF0000FF;
			dt->ticks = 20;
			dt->pos = { 240, 136 };
			txt->addText(dt);

			ItemDrop* drp = new ItemDrop();
			drp->itm = Items::SEEDS;
			drp->quantity = 1;
			drp->pos = { x * 32 + rand() % 10 , y * 32 + rand() % 10 };
			drops->addDrop(drp);
			return;
		}

		if (hitTile->texIndex == 26 && g_Inventory->getItem(hotbarPosition).ID == Items::IRON_HOE.ID && player.energy > 5) {
			TileAnim* t = new TileAnim();
			t->layer = 0;
			t->rgba = GU_COLOR((float)currLevel / 16.0f, (float)currLevel / 16.0f, (float)currLevel / 16.0f, 1.0f);
			t->rotation = 0;
			t->physics = false;
			t->texIndex = 22;
			setTile(x, y, t);

			(*removeAmount) = 5;
			CombatTextDetails* dt = new CombatTextDetails();
			dt->text = std::to_string(8 + rand() % 5);
			dt->color = 0xFF0000FF;
			dt->ticks = 20;
			dt->pos = { 240, 136 };
			txt->addText(dt);

			ItemDrop* drp = new ItemDrop();
			drp->itm = Items::SEEDS;
			drp->quantity = 1;
			drp->pos = { x * 32 + rand() % 10 , y * 32 + rand() % 10 };
			drops->addDrop(drp);

			ItemDrop* drp2 = new ItemDrop();
			drp2->itm = Items::WHEAT;
			drp2->quantity = 1;
			drp2->pos = { x * 32 + rand() % 10 , y * 32 + rand() % 10 };
			drops->addDrop(drp2);

			return;
		}

		//TREEZ

		int chkX = x / 16;
		int chkY = y / 16;

		if (chunkMap.find({ chkX, chkY }) != chunkMap.end()) {

			Chunk* chk = chunkMap[{chkX, chkY}];
			TilemapAnim* tmap = chk->treemap;

			for (int i = 0; i < tmap->size(); i++) {

				if(tmap->getTile(i)->layer == 0){
				glm::vec2 playerPos = (tmap->getTile(i)->offset + tmap->getTile(i)->extent / 2.0f) / 2.0f;
				playerPos.y += 136;

				bool isInRange = ((pos.x - playerPos.x) * (pos.x - playerPos.x)) < (20 * 20) && ((pos.y - playerPos.y) * (pos.y - playerPos.y)) < (20 * 20);
				if (isInRange && g_Inventory->getItem(hotbarPosition).ID == Items::IRON_AXE.ID && player.energy > 5) {


					(*removeAmount) = 5;
					CombatTextDetails* dt = new CombatTextDetails();
					dt->text = "10";
					dt->color = 0xFF0000FF;
					dt->ticks = 20;
					dt->pos = { 240, 136 };
					txt->addText(dt);

					ItemDrop* drp = new ItemDrop();
					drp->itm = Items::LOGS;
					drp->quantity = 3 + rand() % 4;
					drp->pos = { x * 32 + rand() % 10 , y * 32 + rand() % 10 };
					ItemDrop* drp2 = new ItemDrop();
					drp2->itm = Items::ACORN;
					drp2->quantity = 1 + rand() % 4;
					drp2->pos = { x * 32 + rand() % 10 , y * 32 + rand() % 10 };


					ItemDrop* drp3 = new ItemDrop();
					drp3->itm = Items::APPLE;
					drp3->quantity = (1 + rand() % 5) / 5;
					drp3->pos = { x * 32 + rand() % 10 , y * 32 + rand() % 10 };

					drops->addDrop(drp);
					drops->addDrop(drp2);
					drops->addDrop(drp3);

					TileAnim* t2 = new TileAnim();
					t2->isAnim = false;
					t2->indexStart = 0;
					t2->animLength = 0;
					t2->tickNumber = 0;
					t2->rgba = 0;
					t2->layer = -2;
					t2->rotation = 0;
					t2->physics = false;
					t2->offset = {0, 0 };
					t2->extent = { 0, 0 };

					chunkMap[{chkX, chkY}]->treemap->updateTile(i, t2);
					sceKernelDcacheWritebackInvalidateAll();
					chunkMap[{chkX, chkY}]->treemap->buildMap();
					return;
				}
				}
			}
		}
	}
}

World* g_World = NULL;
GameTime g_GameTime = { 0, 0, 0, 16 };