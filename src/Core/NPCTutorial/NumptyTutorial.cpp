#include "NumptyTutorial.h"

NumptyTutorial::NumptyTutorial(glm::vec2 startPosition, char r, Tilemap* tmap1, Tilemap* tmap2, std::string file)
{
	position = startPosition;
	origin = startPosition;

	radius = r;
	ticksTilAction = rand() % 300;
	nextAction = NPC_ACTION_LOOK;

	tex = TextureUtil::LoadPng(file);
	characterModel = new CharacterSprite({ 32, 32 }, new TextureAtlas(4), tex);

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


	characterModel->addAnimEvent("idle", idle);
	characterModel->addAnimEvent("walk", walk);
	characterModel->setDefaultAnimEvent("idle");
	characterModel->triggerAnimEvent("idle");
	characterModel->setFacing(Render2D::CHARACTER_FACING_DOWN);

	controller = new Render2D::TopDownController(characterModel, 128.0f, false);
	controller->getAnimController()->setCharacterTickRateRelative(24);
	controller->setPosition(position);

	controller->addPhysicsTileMap(tmap1);
	controller->addPhysicsTileMap(tmap2);
	ticksMovement = -1;

	movement = { 0, 0 };
}

void NumptyTutorial::simpleAITick()
{
	ticksTilAction--;
	ticksMovement--;

	if (ticksTilAction <= 0) {

		handleAction(nextAction);

		ticksTilAction = rand() % 300;
		nextAction = rand() % 2;
	}
}


void NumptyTutorial::draw()
{
	controller->draw();
}

void NumptyTutorial::update()
{
	if (ticksMovement > 0) {
		controller->velocity = movement * 0.16f;
	}
	else {
		movement = { 0, 0 };
	}
	controller->update(0.16f);
}

void NumptyTutorial::handleAction(char action)
{
	ticksMovement = rand() % 15 + 5;

	if (action == NPC_ACTION_WALK) {
		switch (controller->getCharacterSprite()->getFacing()) {
		case CHARACTER_FACING_DOWN: {
			movement = { 0.f, 64.f };
			break;
		}
		case CHARACTER_FACING_UP: {
			movement = { 0.f, -64.f };
			break;
		}
		case CHARACTER_FACING_LEFT: {
			movement = { -64.f, 0.0f };
			break;
		}
		case CHARACTER_FACING_RIGHT: {
			movement = { 64.f, 0.0f };
			break;
		}
		}

		controller->getCharacterSprite()->triggerAnimEvent("walk");
	}
	else {
		char dir = rand() % 4;
		controller->getCharacterSprite()->setFacing(dir);
	}

}
