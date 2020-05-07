#include "Chunk.h"

Chunk::Chunk(int x, int y)
{
	positionXY = { x, y };
	if (terrain_atlas == NULL) {
		terrain_atlas = TextureUtil::LoadPng("./assets/game/atlas2.png");
	}
	if (treez == NULL) {
		treez = TextureUtil::LoadPng("./assets/game/treez.png");
	}


	treemap = new TilemapAnim(new TextureAtlas(4), treez);
	tmap = new TilemapAnim(new TextureAtlas(16), terrain_atlas);

	if (noise == NULL) {
		noise = new siv::PerlinNoise(std::random_device{});
		srand(time(0));
		noise->reseed(rand());
	}
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
			double sample = noise->accumulatedOctaveNoise2D_0_1((double)(x + positionXY.x * 16) / 16.0f, (double)(y + positionXY.y * 16) / 16.0f, 1) * 2.0f;
			
			TileAnim* t = new TileAnim();
			t->isAnim = false;
			t->rgba = 0xFFFFFFFF;
			t->layer = -1;
			t->rotation = 0;
			t->physics = false;
			t->offset = { x * 32 + (positionXY.x * 16 * 32) , y * 32 + (positionXY.y * 16 * 32) };
			t->extent = { 32, 32 };

			if (sample > 0.72f * 2.0f) {
				t->physics = true;
				t->texIndex = 24 + 3;
			}

			if (sample > 0.5f * 2.0f && sample < 0.57f * 2.0f) {
				srand(x + positionXY.x * 16 * (y + positionXY.y * 16));

				if (rand() % 5 == 0) {

					t->texIndex = 16;
					t->isAnim = true;
					t->indexStart = 16;
					t->tickNumber = rand() % 3;
					t->animLength = 1;
				}
			}

			if (sample < 0.36f * 2.0f) {
				t->texIndex = 18;
				t->isAnim = true;
				t->indexStart = 18;
				t->tickNumber = rand() % 5;
				t->animLength = 3;
			}

			if (sample < 0.29f * 2.0f) {
				t->texIndex = 18 + 16;
				t->isAnim = true;
				t->indexStart = 18 + 16;
				t->tickNumber = rand() % 5;
				t->animLength = 3;
			}

			if (sample > 0.36f * 2.0f && sample < 0.42f * 2.0f) {
				t->texIndex = 26 + 3;
			}


			tmap->addTile(t);

			if (t->texIndex == 0) {
				srand(x + positionXY.x * 16 * (y + positionXY.y * 16) * (y + positionXY.y * 16));
				if (rand() % 20 == 0) {
					TileAnim* t2 = new TileAnim();
					t2->isAnim = true;
					t2->indexStart = 0;
					t2->animLength = 3;
					t2->tickNumber = rand() % 5;
					t2->rgba = 0xFFFFFFFF;
					t2->layer = 0;
					t2->rotation = 0;
					t2->physics = false;
					t2->offset = { (x-1) * 32 + (positionXY.x * 16 * 32) , (y-1) * 32 + (positionXY.y * 16 * 32) };
					t2->extent = { 64, 64 };

					treemap->addTile(t2);
				}
			}

			

		}
	}
	tmap->buildMap();
	treemap->buildMap();
}

void Chunk::saveGenerate()
{
	//LOAD SAVED CHUNKS
}

void Chunk::update()
{
	tmap->tickPhase();
	treemap->tickPhase();
}

void Chunk::draw()
{
	tmap->drawMap();
	treemap->drawMap();
}

Texture* terrain_atlas = NULL;
Texture* treez = NULL;
siv::PerlinNoise* noise = NULL;