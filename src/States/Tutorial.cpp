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


	charSprite = new Render2D::CharacterSprite({ 48, 48 }, atlas, charTexture);

	charSprite->position(240, 136);

	Render2D::CharacterDirectionalAnimInfo* idle = new Render2D::CharacterDirectionalAnimInfo();
	Render2D::CharacterDirectionalAnimInfo* walk = new Render2D::CharacterDirectionalAnimInfo();

	idle->top = { 8, 0 };
	idle->left = { 24, 0 };
	idle->down = { 40, 0 };
	idle->right = { 56,0 };


	walk->top = { 0, 9 };
	walk->left = { 16, 9 };
	walk->down = { 32, 9 };
	walk->right = { 48, 9 };


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

	controller->getAnimController()->setCharacterTickRateRelative(12);
	controller->addPhysicsTileMap(tmap);
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
	controller->draw();

	sceGumMatrixMode(GU_VIEW);
	sceGumLoadIdentity();
	sceGumMatrixMode(GU_MODEL);
	hud->draw();

	inv->drawHotbar();
}
