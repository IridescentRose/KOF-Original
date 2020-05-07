#pragma once
#include <Utilities/Logger.h>
#include <Graphics/2D/TilemapAnim.h>
#include <Graphics/2D/CharacterSprite.h>
#include <Graphics/UI/UIText.h>
#include "Perlin.h"

using namespace Stardust;
using namespace Stardust::Graphics::Render2D;
using namespace Stardust::Graphics;
using namespace Stardust::Graphics::UI;

class Chunk {
public:
	Chunk(int x, int y);
	~Chunk();

	void generate();
	void saveGenerate();

	void update();
	void draw();
	
private:
	glm::vec2 positionXY;
	TilemapAnim* tmap;
	TilemapAnim* treemap;
	

};

extern Texture* terrain_atlas;
extern Texture* treez;
extern siv::PerlinNoise* noise;