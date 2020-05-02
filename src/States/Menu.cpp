#include "Menu.h"

int state;

MenuState::MenuState()
{
}

void MenuState::init()
{
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

	start = new UI::UIButton({ 240, 136 }, { 180, 32 }, unsel, sel, strt);
	start->setPosition({ 240, 100 });
	instr = new UI::UIButton({ 240, 136 }, { 180, 32 }, unsel, sel, inst);
	instr->setPosition({ 240, 140 });
	credit = new UI::UIButton({ 240, 136 }, { 180, 32 }, unsel, sel, crdt);
	credit->setPosition({ 240, 180 });
	exits = new UI::UIButton({ 240, 136 }, { 180, 32 }, unsel, sel, exit);
	exits->setPosition({ 240, 220 });
}

void MenuState::cleanup()
{
}

void MenuState::update(GameStateManager* st)
{
	stateMan = st;
}

void MenuState::draw(GameStateManager* st)
{
	logo->draw();
	start->draw();
	instr->draw();
	credit->draw();
	exits->draw();
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