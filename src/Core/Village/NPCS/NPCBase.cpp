#include "NPCBase.h"
#include "../../../World/World.h"

NPCBase::NPCBase(glm::vec2 startPosition, char r, TilemapAnim* tmap1, TilemapAnim* tmap2, std::string file, std::string t)
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
	characterModel->triggerAnimEvent("walk");
	characterModel->setFacing(Render2D::CHARACTER_FACING_DOWN);

	controller = new Render2D::TopDownController(characterModel, 128.0f, false);
	controller->getAnimController()->setCharacterTickRateRelative(24);
	controller->setPosition(position);

	controller->addPhysicsTileMap(tmap1);
	controller->addPhysicsTileMap(tmap2);
	ticksMovement = -1;

	movement = { 0, 0 };
	tag = t;
}

void NPCBase::simpleAITick()
{
	ticksTilAction--;
	ticksMovement--;

	if (ticksTilAction <= 0) {

		handleAction(nextAction);

		ticksTilAction = rand() % 300;

		if (tag != "settler" || movedIn) {

			nextAction = NPC_ACTION_LOOK;
		}
		else {
			nextAction = rand() % 2;
		}
	}
}

void NPCBase::draw()
{
	controller->getCharacterSprite()->setColor(GU_COLOR((float)g_GameTime.lightLevel / 16.0f, (float)g_GameTime.lightLevel / 16.0f, (float)g_GameTime.lightLevel / 16.0f, 1.0f));
	controller->draw();
}

void NPCBase::update()
{
	if(tag != "settler" || movedIn){
		if (ticksMovement > 0) {
			controller->velocity = movement * 0.16f;
		}
		else {
			movement = { 0, 0 };
		}

		position = controller->getCharacterSprite()->getPosition();
		controller->update(0.16f);
	}
}

void NPCBase::handleAction(char action)
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

Dialog* NPCBase::getDialog()
{
	Dialog* d = new Dialog();

	d->interactionType = INTERACTION_TYPE_NONE;
	d->text = "Placeholder";


	return d;
}
