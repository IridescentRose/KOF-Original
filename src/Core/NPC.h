#pragma once
#include <Graphics/2D/SpriteBase.h>

using namespace Stardust;
using namespace Stardust::Graphics;
using namespace Stardust::Graphics::Render2D;

#include <Graphics/2D/CharacterSprite.h>
#include <Graphics/2D/TopDownController.h>

enum NPCAction {
	NPC_ACTION_WALK = 0,
	NPC_ACTION_LOOK = 1,
};

class NPC {
public:
	virtual void simpleAITick() = 0;
	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void handleAction(char action) = 0;

protected:
	glm::vec2 position;
	char radius;
	glm::vec2 origin;

	int ticksTilAction;
	char nextAction;

	int ticksMovement;

	glm::vec2 movement;

	Texture* tex;
	CharacterSprite* characterModel;
	TopDownController* controller;
};