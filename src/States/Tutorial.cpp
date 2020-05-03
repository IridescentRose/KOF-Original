#include "Tutorial.h"

TutorialState::TutorialState()
{
}

void TutorialState::init()
{
	texAtlas = TextureUtil::LoadPng("./assets/game/atlas.png");
	atlas = new TextureAtlas(16);
	tmap = new Render2D::Tilemap(atlas, texAtlas);
	tmap->loadTileFromJSON("./assets/game/tutorial.json");
	tmap->buildMap();
}

void TutorialState::cleanup()
{
}

void TutorialState::enter()
{
}

void TutorialState::pause()
{
}

void TutorialState::resume()
{
}

void TutorialState::update(GameStateManager* st)
{
}

void TutorialState::draw(GameStateManager* st)
{
	tmap->drawMap();
}
