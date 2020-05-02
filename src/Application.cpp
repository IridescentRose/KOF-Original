#include "Application.h"

Application::Application()
{

}

Application::~Application()
{
}

void Application::run()
{
	//We'll initialize stuff here.
	gsm = new GameStateManager();

	//Set Up Some Sort of chain loading
	//gsm->changeState();
}

void Application::update()
{
	gsm->update();
}

void Application::draw()
{
	gsm->draw();
}
