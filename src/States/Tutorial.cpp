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
	Utilities::app_Logger->log("HI");
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

	if (Utilities::KeyPressed(PSP_CTRL_START)) {
		pauseHandler();
	}


	controller->update(0.016f);
}

void TutorialState::draw(GameStateManager* st)
{
	controller->draw();
	tmap->drawMap();
	treemap->drawMap();
	controller->draw();

	sceGumMatrixMode(GU_VIEW);
	sceGumLoadIdentity();
	sceGumMatrixMode(GU_MODEL);
	hud->draw();

	inv->drawHotbar();
}
