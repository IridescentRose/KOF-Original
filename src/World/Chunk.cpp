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
			t->offset = { x * 32 + (positionXY.x * 16 * 32) , y * 32 + (positionXY.y * 16 * 32) - 272 };
			t->extent = { 32, 32 };

			bool spawnpoint = (positionXY == glm::vec2(50, 50)) && x <3 && y < 3;

			if(!spawnpoint){
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



			if (sample > 0.5f * 2.0f && sample < 0.57f * 2.0f) {
				srand(-x + positionXY.x * 16 * (-y + positionXY.y * 16));

				if (rand() % 5 == 0) {

					t->texIndex = 16 + 16;
					t->isAnim = true;
					t->indexStart = 16 + 16;
					t->tickNumber = rand() % 3;
					t->animLength = 1;
					t->physics = false;
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
			}
			


			tmap->addTile(t);

			if (t->texIndex == 0 && !t->isAnim) {
				if (rand()*x*y % 40 == 0&& !spawnpoint) {
					TileAnim* t2 = new TileAnim();
					t2->isAnim = true;
					t2->indexStart = 0;
					t2->animLength = 3;
					t2->tickNumber = rand() % 5;
					t2->rgba = 0xFFFFFFFF;
					t2->layer = 0;
					t2->rotation = 0;
					t2->physics = true;
					t2->offset = { (x) * 32 + (positionXY.x * 16 * 32) , (y-1) * 32 + (positionXY.y * 16 * 32) - 272};
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
	ScePspFVector3 v = { 0, 272, 0 };
	sceGumTranslate(&v);
	tmap->drawMap();
	treemap->drawMap();
	sceGumPopMatrix();
}

void Chunk::setLighting(int level)
{
	for (int i = 0; i < tmap->size(); i++) {
		auto tile = tmap->getTile(i);
		tile->rgba = GU_COLOR((float)level / 16.0f, (float)level / 16.0f, (float)level / 16.0f, 1.0f);
	}
	for (int i = 0; i < treemap->size(); i++) {
		auto tile = treemap->getTile(i);
		tile->rgba = GU_COLOR((float)level / 16.0f, (float)level / 16.0f, (float)level / 16.0f, 1.0f);
	}
	tmap->buildMap();
	treemap->buildMap();

	lightLevel = level;
}

void Chunk::updateTiles()
{
}

Texture* terrain_atlas = NULL;
Texture* treez = NULL;
siv::PerlinNoise* noise = NULL;