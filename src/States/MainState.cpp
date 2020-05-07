#include "MainState.h"

MainState::MainState()
{
}

void MainState::init()
{
	g_World = new World();
}

void MainState::cleanup()
{
}

void MainState::enter()
{
}

void MainState::pause()
{
}

void MainState::resume()
{
}

void MainState::update(GameStateManager* st)
{
	g_World->update();
}

void MainState::draw(GameStateManager* st)
{
	g_World->draw();
}
