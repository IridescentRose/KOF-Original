#pragma once
#include <map>
#include "Chunk.h"
#include <Graphics/2D/SpriteBase.h>
#include <Graphics/2D/TopDownController.h>
#include "../States/Tutorial.h"
#include <Utilities/Timer.h>
#include "Vector3.h"

using namespace Stardust::Graphics;

class World {
public:
	World();
	~World();

	void update();
	void draw();

private:
	Texture* charTexture;
	TextureAtlas* atlas;
	Render2D::CharacterSprite* charSprite;
	Render2D::TopDownController* controller;
	DropManager* drops;
	HUD* hud;
	std::map<Vector3i, Chunk*> chunkMap;

	glm::ivec2 lastPos;
};

extern World* g_World;