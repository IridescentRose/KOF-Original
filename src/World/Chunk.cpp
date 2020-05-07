#include "Chunk.h"

Chunk::Chunk(int x, int y)
{
	positionXY = { x, y };
	if (terrain_atlas == NULL) {
		terrain_atlas = TextureUtil::LoadPng("./assets/game/atlas.png");
	}
	if (treez == NULL) {
		treez = TextureUtil::LoadPng("./assets/game/tree.png");
	}


	treemap = new TilemapAnim(new TextureAtlas(1), treez);
	tmap = new TilemapAnim(new TextureAtlas(16), terrain_atlas);

	if (noise == NULL) {
		noise = new siv::PerlinNoise(std::random_device{});
		srand(time(0));
		noise->reseed(std::mt19937{ rand() });
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
			t->offset = { x * 32, y * 32 };
			t->extent = { 32, 32 };

			if (sample > 0.72f * 2.0f) {
				t->physics = true;
				t->texIndex = 23;
			}

			if (sample > 0.5f * 2.0f && sample < 0.57f * 2.0f) {
				srand(x + positionXY.x * 16 * (y + positionXY.y * 16));

				if(rand() % 5 == 0)
					t->texIndex = 16;
			}

			if (sample < 0.36f * 2.0f) {
				t->texIndex = 17;
			}

			if (sample > 0.36f * 2.0f && sample < 0.42f * 2.0f) {
				t->texIndex = 25;
			}


			tmap->addTile(t);

			if (t->texIndex == 0) {
				srand(x + positionXY.x * 16 * (y + positionXY.y * 16) * (y + positionXY.y * 16));
				if (rand() % 20 == 0) {
					TileAnim* t2 = new TileAnim();
					t2->isAnim = false;
					t2->rgba = 0xFFFFFFFF;
					t2->layer = 0;
					t2->rotation = 0;
					t2->physics = false;
					t2->offset = { (x-1) * 32, (y-1) * 32 };
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
	sceGumPushMatrix();
	ScePspFVector3 v = { positionXY.x * (32 * 16), positionXY.y * (32 * 16), 0 };
	sceGumTranslate(&v);
	tmap->drawMap();
	treemap->drawMap();
	sceGumPopMatrix();
}

Texture* terrain_atlas = NULL;
Texture* treez = NULL;
siv::PerlinNoise* noise = NULL;