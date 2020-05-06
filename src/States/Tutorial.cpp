#include "Tutorial.h"
#include <Utilities/Input.h>

TutorialState::TutorialState()
{
}

void TutorialState::init()
{
	srand(time(0));
	texAtlas = TextureUtil::LoadPng("./assets/game/atlas.png");
	charTexture = TextureUtil::LoadPng("./assets/game/mc.png");
	atlas = new TextureAtlas(16);

	tmap = new Render2D::Tilemap(atlas, texAtlas);
	tmap->loadTileFromJSON("./assets/game/tutorial.json");
	tmap->buildMap();

	tree = TextureUtil::LoadPng("./assets/game/tree.png");

	treemap = new Render2D::Tilemap(new TextureAtlas(1), tree);

	Tile* t = new Tile();
	t->offset = { 60 * 2 * 16, 44 * 2 * 16 };
	t->extent = { 48, 48 };
	t->layer = 0;
	t->rgba = 0xFFFFFFFF;
	t->rotation = 0;
	t->physics = true;
	t->texIndex = 0;


	Tile* t2 = new Tile();
	t2->offset = { 56 * 2 * 16, 43 * 2 * 16 };
	t2->extent = { 48, 48 };
	t2->layer = 0;
	t2->rgba = 0xFFFFFFFF;
	t2->rotation = 0;
	t2->physics = true;
	t2->texIndex = 0;


	Tile* t3 = new Tile();
	t3->offset = { 52 * 2 * 16, 45 * 2 * 16 };
	t3->extent = { 48, 48 };
	t3->layer = 0;
	t3->rgba = 0xFFFFFFFF;
	t3->rotation = 0;
	t3->physics = true;
	t3->texIndex = 0;

	Tile* t4 = new Tile();
	t4->offset = { 49 * 2 * 16, 47 * 2 * 16 };
	t4->extent = { 48, 48 };
	t4->layer = 0;
	t4->rgba = 0xFFFFFFFF;
	t4->rotation = 0;
	t4->physics = true;
	t4->texIndex = 0;

	treemap->addTile(t);
	treemap->addTile(t2);
	treemap->addTile(t3);
	treemap->addTile(t4);
	treemap->buildMap();

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
	controller->addPhysicsTileMap(tmap);
	controller->addPhysicsTileMap(treemap);
	controller->setPosition({ 32 * 24, 32 * 27 });

	hotbarPosition = 0;

	hud = new HUD();
	g_Inventory = new Inventory();

	npcs.clear();
	
	npcs.push_back(new NumptyTutorial({ 32 * 24, 32 * 37 }, 3, tmap, treemap));
	npcs.push_back(new NumptyTutorial({ 32 * 41, 32 * 27 }, 3, tmap, treemap));
	npcs.push_back(new NumptyTutorial({ 32 * 30, 32 * 30 }, 3, tmap, treemap, "./assets/game/NPC/settler.png", "settler"));
	npcs.push_back(new NumptyTutorial({ 32 * 52, 32 * 27 }, 3, tmap, treemap, "./assets/game/NPC/guard.png", "guard"));
	npcs.push_back(new NumptyTutorial({ 32 * 8, 32 * 18 }, 3, tmap, treemap, "./assets/game/NPC/farmer.png", "farmer"));
	npcs.push_back(new NumptyTutorial({ 32 * 8, 32 * 54 }, 3, tmap, treemap, "./assets/game/NPC/miner.png", "miner"));
	npcs.push_back(new NumptyTutorial({ 32 * 57, 32 * 54 }, 3, tmap, treemap, "./assets/game/NPC/lumberjack.png", "lumberjack"));

	dialog = new Dialogue();
	Dialog* d2 = new Dialog(); 
	d2->text = "Tutorial: Settlers wear white clothing!\nGo find the settler and press X to interact!";
	d2->interactionType = INTERACTION_TYPE_NONE;

	Dialog* d = new Dialog(); 
	d->text = "You: Today's the day. The day I finally can begin to set out\non my journey. I've waited for years for the settler to\nreturn.He promised we could start a new village. A place\nwhere I can begin my own life, my own land.\nIt's been a long time. I should go see him...";
	d->interactionType = INTERACTION_TYPE_NONE;
	

	dial = new DialogStack(dialog);
	dial->addDialog(d);
	dial->addDialog(d2);

	txt = new CombatText();
	prevEngage = false;

	player.energy = 10;
	player.health = 20.5f;
	player.hunger = 20.5f;
	player.gold = 100;
	drops = new DropManager();
}

void TutorialState::cleanup()
{
}

void TutorialState::enter()
{
}

void TutorialState::pause()
{
}

void TutorialState::resume()
{
}

void TutorialState::update(GameStateManager* st)
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
	}else{
		controller->speed = 128.0f;
	}

	bool removeEnergy = false;

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
			for (auto npc : npcs) {
				glm::vec2 pos = npc->getPosition();
				glm::vec2 playerPos = controller->getCharacterSprite()->getPosition();

				if ( ((pos.x - playerPos.x) * (pos.x - playerPos.x)) < (16*16) && ((pos.y - playerPos.y) * (pos.y - playerPos.y)) < (16 * 16)) {
					//INTERACT
					dial->addDialog(npc->getDialog());
				}

			}

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

				if ((g_Inventory->getItem(hotbarPosition).ID == Items::BREAD.ID || g_Inventory->getItem(hotbarPosition).ID == Items::APPLE.ID)&& player.energy > 5) {
					if (player.hunger < 20.5f) {
						player.hunger += 5;
						player.health += 2;
					}
					g_Inventory->getItemSlot(hotbarPosition)->quantity--;
					if (g_Inventory->getItemSlot(hotbarPosition)->quantity == 0) {
						g_Inventory->getItemSlot(hotbarPosition)->item = Items::NONE;
					}
				}

				if (tmap->getTile(x + y * 64)->texIndex > 18 && tmap->getTile(x + y * 64)->texIndex <= 22 && g_Inventory->getItem(hotbarPosition).ID == Items::IRON_HOE.ID && player.energy > 5) {
					Tile* t = new Tile();
					t->offset = { x * 32, y * 32 };
					t->layer = 0;
					t->rgba = 0xFFFFFFFF;
					t->rotation = 0;
					t->physics = false;
					t->extent = { 32, 32 };
					t->texIndex = 18;
					tmap->updateTile(x + y * 64, t);
					tmap->buildMap();

					removeAmount = 5;
					CombatTextDetails* dt = new CombatTextDetails();
					dt->text = "10";
					dt->color = 0xFF0000FF;
					dt->ticks = 20;
					dt->pos = { 240, 136 };
					txt->addText(dt);

					ItemDrop* drp = new ItemDrop();
					drp->itm = Items::WHEAT;
					drp->quantity = 1;
					drp->pos = { x * 32 + rand() % 10 , y * 32 + rand() % 10 };

					ItemDrop* drp2 = new ItemDrop();
					drp2->pos = { x * 32 + rand() % 10 , y * 32 + rand() % 10 };
					drp2->itm = Items::SEEDS;
					drp2->quantity = 1 + rand() % 3;

					if (tmap->getTile(x + y * 64)->texIndex <= 21) {
						drp2->quantity = 1;
						drops->addDrop(drp2);
					}
					else {
						drops->addDrop(drp);
						drops->addDrop(drp2);
					}

					progInfo.canCompleteFarmer = true;

				}

				if (tmap->getTile(x + y * 64)->texIndex == 23 && g_Inventory->getItem(hotbarPosition).ID == Items::IRON_PICKAXE.ID && player.energy > 5) {
					Tile* t = new Tile();
					t->offset = { x * 32, y * 32 };
					t->layer = 0;
					t->rgba = 0xFFFFFFFF;
					t->rotation = 0;
					t->physics = false;
					t->extent = { 32, 32 };
					t->texIndex = 0;
					tmap->updateTile(x + y * 64, t);
					tmap->buildMap();

					removeAmount = 5;
					CombatTextDetails* dt = new CombatTextDetails();
					dt->text = "10";
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

				for (int i = 0; i < 4; i++) {
					auto tree = treemap->getTile(i);

					if (tree->rgba > 0) {

						glm::vec2 playerPos = (tree->offset + tree->extent / 2.0f) / 2.0f;
						Utilities::app_Logger->log("TREE " + std::to_string(playerPos.x) + " " + std::to_string(playerPos.y));
						Utilities::app_Logger->log("PLAYER " + std::to_string(pos.x) + " " + std::to_string(pos.y));
						bool isInRange = ((pos.x - playerPos.x) * (pos.x - playerPos.x)) < (32 * 32) && ((pos.y - playerPos.y) * (pos.y - playerPos.y)) < (32 * 32);

						if (isInRange && g_Inventory->getItem(hotbarPosition).ID == Items::IRON_AXE.ID && player.energy > 5) {
							tree->rgba = 0;
							tree->physics = false;

							removeAmount = 5;
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
								

							progInfo.canCompleteLumber = true;

							drops->addDrop(drp);
							drops->addDrop(drp2);
							drops->addDrop(drp3);

							treemap->buildMap();
						}


					}
				}

			}
		}
	}

	if (progInfo.talkToSettler) {
		if (!progInfo.tutorialTypesTriggered) {
			progInfo.tutorialTypesTriggered = true;
			Dialog* d = new Dialog();
			d->interactionType = INTERACTION_TYPE_NONE;
			d->text = "Tutorial: Each class of villager is a different color! \nFarmers are orange. Miners are grey. Guards are blue.\nLumberjacks are dark green. There are many more to meet!";
			dial->addDialog(d);
		}
	}
	if (progInfo.talkFarmer) {
		if (!progInfo.tutorialFarmTriggered) {
			progInfo.tutorialFarmTriggered = true;
			Dialog* d = new Dialog();
			d->interactionType = INTERACTION_TYPE_NONE;
			d->text = "Tutorial: Go to the wheat farms and equip your hoe.\nPress X to harvest!\nFully golden wheat will drop drop seeds and wheat!\nWheat can be made into bread for food.\nWe'll plant some later!";
			dial->addDialog(d);
		}
	}
	if (progInfo.talkGuard) {
		if (!progInfo.tutorialCombatTriggered) {
			progInfo.tutorialCombatTriggered = true;
			Dialog* d = new Dialog();
			d->interactionType = INTERACTION_TYPE_NONE;
			d->text = "Tutorial: Go find the bandit and take him down!\nWith sword equipped, hitting X will swing your sword!\nBandits are in black.";
			dial->addDialog(d);

			npcs.push_back(new NumptyTutorial({ 32 * 52, 32 * 10 }, 3, tmap, treemap, "./assets/game/NPC/bandit.png", "bandit"));
		}
	}
	if (progInfo.talkMiner) {
		if (!progInfo.tutorialMineTriggered) {
			progInfo.tutorialMineTriggered = true;
			Dialog* d = new Dialog();
			d->interactionType = INTERACTION_TYPE_NONE;
			d->text = "Tutorial: Go to the quarry and equip your pickaxe!\nHitting X will break the stone! You'll need a few hits!";
			dial->addDialog(d);
		}
	}
	if (progInfo.talkLumber) {
		if (!progInfo.tutorialLumberTriggered) {
			progInfo.tutorialLumberTriggered = true;
			Dialog* d = new Dialog();
			d->interactionType = INTERACTION_TYPE_NONE;
			d->text = "Tutorial: Equip your axe and go to the tree farm!\nHit X to cut the trees! You'll need a few hits!";
			dial->addDialog(d);
		}
	}

	dialog->update();
	dial->update();

	if ((dialog->isEngaged() || g_Inventory->isEngaged()) != prevEngage) {
		if (progInfo.tutorialCompleted) {
			//END OF TUTORIAL!
		}

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

	if (Utilities::KeyPressed(PSP_CTRL_START)) {
		pauseHandler();
	}

	for (auto npc : npcs) {
		npc->simpleAITick();
		npc->update();
	}
	txt->update();
	controller->update(0.016f);
	hud->update();
	drops->update(controller->getCharacterSprite()->getPosition());
}

void TutorialState::draw(GameStateManager* st)
{
	controller->draw();
	tmap->drawMap();
	treemap->drawMap();

	for (auto npc : npcs) {
		npc->draw();
	}

	drops->draw();
	controller->draw();
	

	g_RenderCore.Set2DMode();

	txt->draw();

	//UI
	hud->draw();
	g_Inventory->drawHotbar();
	dialog->draw();
	g_Inventory->draw();
}
PlayerInfo player;
TutProgInfo progInfo = { false, false, false, false, false , false, false, false, false , false, false, false, false, false , false, false, false, false};