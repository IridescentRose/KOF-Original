#pragma once
#include <map>
#include "Chunk.h"
#include <Graphics/2D/SpriteBase.h>
#include <Graphics/2D/TopDownController.h>
#include "../States/Tutorial.h"
#include <Utilities/Timer.h>
#include "Vector3.h"
#include "../Core/Village/Village.h"

using namespace Stardust::Graphics;

struct GameTime {
	u32 dayTime;
	u32 totalTime;

	u32 days;
	u8 lightLevel;
};

class Item;
class Items;
class ItemSprites;
class ItemDrop;
class DropManager;
class Dialogue;
class DialogStack;
class CombatText;

class World {
public:
	World();
	~World();

	void update();
	void draw();

	void respawn();

	TileAnim* getTile(int x, int y);
	void setTile(int x, int y, TileAnim* t);


	std::map<Vector3i, Chunk*> chunkMap;
private:
	Texture* charTexture;
	TextureAtlas* atlas;
	Render2D::CharacterSprite* charSprite;
	Render2D::TopDownController* controller;
	DropManager* drops;
	HUD* hud;
	UIText* debugRAM;
	int hotbarPosition;
	glm::ivec2 lastPos;
	Dialogue* dialog;
	DialogStack* dial;
	bool prevEngage;
	CombatText* txt;
	int currDay;
	int currLevel;

	glm::vec2 playerRespawnPos;

	void playerUpdate();
	void animUpdate();
	void chunkgenUpdate();
	void leftClickInteract(int x, int y, glm::vec2 position, int* removeAmount);
};

extern World* g_World;
extern GameTime g_GameTime;