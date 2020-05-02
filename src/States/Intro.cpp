#include "Intro.h"
#include <Utilities/Logger.h>


IntroState::IntroState()
{
}

void IntroState::init()
{
	dialog = new GameDialog("Made by");
	dialog->setSpeed(8);
	dialog->setPosition({ 240, 136 });
	dialog->reset();

	timer = new Stardust::Utilities::Timer();
	timer->reset();

	tex = TextureUtil::LoadPng("assets/logo/iridescence.png");
	logo = new Render2D::Sprite(tex);
	logo->SetPosition(240, 136);
	sceGuEnable(GU_BLEND);
	stage = 0;

}

void IntroState::cleanup()
{
	delete dialog;
	delete tex;
	delete logo;
}



void IntroState::update(GameStateManager* st)
{
}

void IntroState::draw(GameStateManager* st)
{
	timer->deltaTime();

	
	if (timer->elapsed() > 2.0f && timer->elapsed() < 4.0f) {
		logo->Draw();

		dialog->setText("Using");
		dialog->reset();
		stage = 1;
	}
	else if (timer->elapsed() > 5.0f && timer->elapsed() < 7.0f) {
		logo->Draw();

		dialog->setText("For");
		dialog->reset();
		stage = 3;
	}else if (timer->elapsed() > 8.0f && timer->elapsed() < 10.0f) {
		logo->Draw();

		dialog->setText("");
		dialog->reset();
		stage = 5;
	}
	else {
		changeState1();
		dialog->animateStep();
		dialog->draw();

		if (stage == 5) {
			MenuState* str = new MenuState();
			str->init();

			st->changeState(str);
		}
	}

}

void IntroState::changeState1()
{
	if (stage == 1) {
		stage++;
		delete tex;
		tex = TextureUtil::LoadPng("assets/logo/stardustLogo.png");
		logo->tex = tex;
	}

	if (stage == 3) {
		stage++;
		delete tex;
		tex = TextureUtil::LoadPng("assets/logo/awakening.png");
		logo->tex = tex;
	}
}


void IntroState::enter()
{
}

void IntroState::pause()
{
}

void IntroState::resume()
{
}