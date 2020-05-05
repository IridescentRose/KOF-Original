#include "Tutorial.h"
#include <Utilities/Input.h>

TutorialState::TutorialState()
{
}

void TutorialState::init()
{
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
	inv = new Inventory();

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

	prevEngage = false;
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
	if (!dialog->isEngaged()) {
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

		inv->setHotbarSelect(hotbarPosition);

		if (Utilities::KeyPressed(PSP_CTRL_CROSS)) {
			for (auto npc : npcs) {
				glm::vec2 pos = npc->getPosition();
				glm::vec2 playerPos = controller->getCharacterSprite()->getPosition();

				if ( ((pos.x - playerPos.x) * (pos.x - playerPos.x)) < (16*16) && ((pos.y - playerPos.y) * (pos.y - playerPos.y)) < (16 * 16)) {
					//INTERACT
					dial->addDialog(npc->getDialog());
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
			d->text = "Tutorial: Go find the bandit and take him down!\nWith sword equipped, hitting X will swing your sword!";
			dial->addDialog(d);
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

	if (dialog->isEngaged() != prevEngage) {
		if (dialog->isEngaged()) {
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
	prevEngage = dialog->isEngaged();

	if (Utilities::KeyPressed(PSP_CTRL_START)) {
		pauseHandler();
	}

	for (auto npc : npcs) {
		npc->simpleAITick();
		npc->update();
	}
	
	controller->update(0.016f);
	dialog->update();
	dial->update();
}

void TutorialState::draw(GameStateManager* st)
{
	controller->draw();
	tmap->drawMap();
	treemap->drawMap();

	for (auto npc : npcs) {
		npc->draw();
	}

	controller->draw();


	g_RenderCore.Set2DMode();

	//UI
	hud->draw();
	inv->drawHotbar();
	dialog->draw();
}

TutProgInfo progInfo = { false, false, false, false, false , false, false, false, false , false, false, false, false, false , false, false, false, false};