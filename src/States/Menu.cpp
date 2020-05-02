#include "Menu.h"
#include <Utilities/Input.h>
#include <Utilities/Logger.h>
int state;
int selected;

using namespace Stardust::Utilities;

MenuState::MenuState()
{
}

void MenuState::init()
{
	state = MENU_MAIN;
	selected = 0;
	unsel = TextureUtil::LoadPng("./assets/main/unselected.png");
	sel = TextureUtil::LoadPng("./assets/main/selected.png");
	log = TextureUtil::LoadPng("./assets/main/logo.png");

	logo = new Render2D::Sprite2(log, { 512, 512 });
	logo->position(240, 40);
	g_RenderCore.SetClearColor(0, 0, 32, 255);

	strt = new UI::UIText({ 240, 136 }, "Start");
	inst = new UI::UIText({ 240, 136 }, "Instructions");
	crdt = new UI::UIText({ 240, 136 }, "Credits");
	exit = new UI::UIText({ 240, 136 }, "Exit");

	instructions = new UI::UIText({ 240, 0 }, "Instructions:\nWelcome to your new adventure.\nYou live in the city of Easthaven.\nYour heart aches for adevnture\nBut you must acquire the funds\nto start your journey in the wild.\nEasthaven is a strange community.\nWhere the living and the undead\nWill rest in harmony.\n\nDPAD - Move, X - Interact\n O - Exit, TRI - Inventory\nR/L - Hotswap, START - Menu\n SQUARE - Toggle Build\n\nPress X to return.");
	credits = new UI::UIText({ 240, 40 }, "Game Made by Nathan Bourgeois\n\nTextures: Nathan Bourgeois\nOther Assets from sources\n(see LICENSES.txt)\n\nFor: The PSP Homebrew Community\n\n\n\n\nPress X to return.");

	start = new UI::UIButton({ 240, 136 }, { 180, 32 }, unsel, sel, strt);
	start->setPosition({ 240, 100 });
	instr = new UI::UIButton({ 240, 136 }, { 180, 32 }, unsel, sel, inst);
	instr->setPosition({ 240, 140 });
	credit = new UI::UIButton({ 240, 136 }, { 180, 32 }, unsel, sel, crdt);
	credit->setPosition({ 240, 180 });
	exits = new UI::UIButton({ 240, 136 }, { 180, 32 }, unsel, sel, exit);
	exits->setPosition({ 240, 220 });

	mainTheme = new Audio::AudioClip("./assets/main/maintheme.bgm", true);
	mainTheme->Play(7);

	click = new Audio::AudioClip("./assets/main/click.wav");
}

void MenuState::cleanup()
{
}

void MenuState::update(GameStateManager* st)
{
	stateMan = st;

	switch (state) {
	case MENU_MAIN: {
		if (KeyPressed(PSP_CTRL_UP)) {
			selected--;
			click->Play(0);
		}
		else if(KeyPressed(PSP_CTRL_DOWN)){
			selected++;
			click->Play(0);
		}


		if (selected < 0) {
			selected = 0;
		}
		if (selected > 3) {
			selected = 3;
		}

		if (KeyPressed(PSP_CTRL_CROSS)) {
			
			switch (selected) {
			case 0: {
				//LOAD GAME!
				break;
			}

			case 1: {
				//INSTRUCTIONS
				selected = 0;
				state = MENU_INSTRUCTION;
				click->Play(0);
				break;
			}
			case 2: {
				//CREDITS
				selected = 0;
				state = MENU_CREDITS;
				click->Play(0);
				break;
			}
			case 3: {
				sceKernelExitGame();
				break;
			}

			}
		}
		app_Logger->log("SEL: " + std::to_string(selected));
		break;
	}

	case MENU_CREDITS: {
		if (KeyPressed(PSP_CTRL_CROSS)) {
			selected = 0;
			state = MENU_MAIN;
			click->Play(0);
		}
		break;
	}

	case MENU_INSTRUCTION: {
		if (KeyPressed(PSP_CTRL_CROSS)) {
			selected = 0;
			state = MENU_MAIN;
			click->Play(0);
		}
		break;
	}
	}
}

void MenuState::draw(GameStateManager* st)
{
	switch (state) {
	case MENU_MAIN: {

		logo->draw();
		start->draw();
		instr->draw();
		credit->draw();
		exits->draw();

		switch (selected) {
		case 0: {
			start->setSelected();
			instr->setUnselected();
			credit->setUnselected();
			exits->setUnselected();
			break;
		}
		case 1: {
			start->setUnselected();
			instr->setSelected();
			credit->setUnselected();
			exits->setUnselected();
			break;
		}
		case 2: {
			start->setUnselected();
			instr->setUnselected();
			credit->setSelected();
			exits->setUnselected();
			break;
		}
		case 3: {
			start->setUnselected();
			instr->setUnselected();
			credit->setUnselected();
			exits->setSelected();
			break;
		}

		}

		break;
	}

	case MENU_CREDITS: {
		credits->draw();
		break;
	}
	case MENU_INSTRUCTION: {
		instructions->draw();
		break;
	}
	}
}


//UNUSED
void MenuState::enter()
{
}

void MenuState::pause()
{
}

void MenuState::resume()
{
}