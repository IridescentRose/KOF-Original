#include "Application.h"
#include "States/Intro.h"

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

#ifndef KOF_DEBUG
	//Set Up Some Sort of chain loading
	IntroState* st = new IntroState();
	st->init();
	gsm->changeState(st);
#else
	g_Inventory = new Inventory();
	MainState* tst = new MainState();
	tst->init();


	gsm->changeState(tst);

#endif
}

void Application::update()
{
	gsm->update();
}

void Application::draw()
{
	gsm->draw();
}
