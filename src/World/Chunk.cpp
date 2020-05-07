#include "Chunk.h"

Chunk::Chunk(int x, int y)
{
	positionXY = { x, y };
	if (terrain_atlas == NULL) {
		terrain_atlas = TextureUtil::LoadPng("./assets/game/atlas.png");
	}
	tmap = new TilemapAnim(new TextureAtlas(16), terrain_atlas);
}

Chunk::~Chunk()
{
	tmap->clearTiles();
	delete tmap;
}

void Chunk::generate()
{
	for (int y = 0; y < 16; y++) {
		for (int x = 0; x < 16; x++) {
			TileAnim* t = new TileAnim();
			t->isAnim = false;
			t->rgba = 0xFFFFFFFF;
			t->layer = 0;
			t->rotation = 0;
			t->physics = false;
			t->offset = { x * 32, y * 32 };
			t->extent = { 32, 32 };
			
			tmap->addTile(t);
		}
	}
	tmap->buildMap();
}

void Chunk::saveGenerate()
{
	//LOAD SAVED CHUNKS
}

void Chunk::update()
{
	tmap->tickPhase();

}

void Chunk::draw()
{
	sceGumPushMatrix();
	ScePspFVector3 v = { positionXY.x * (32 * 16), positionXY.y * (32 * 16), 0 };
	sceGumTranslate(&v);
	tmap->drawMap();
	sceGumPopMatrix();
}

Texture* terrain_atlas = NULL;