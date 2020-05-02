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
	dialog = new GameDialog("Hello World");
	dialog->setSpeed(5);
	dialog->setPosition({ 240, 136 });
	dialog->reset();

	//Set Up Some Sort of chain loading
	//gsm->changeState();
}

void Application::update()
{
	gsm->update();
	dialog->animateStep();
}

void Application::draw()
{
	gsm->draw();
	dialog->draw();
}
