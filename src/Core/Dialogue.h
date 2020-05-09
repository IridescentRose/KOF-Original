#pragma once
#include <Graphics/2D/SpriteBase.h>
#include "../GameDialog.h"
#include "Item.h"

using namespace Stardust;
using namespace Stardust::Graphics;
using namespace Stardust::Graphics::Render2D;

enum InteractionType {
	INTERACTION_TYPE_NONE,
	INTERACTION_TYPE_CONFIRM,
	INTERACTION_MULTISELECT,
	INTERACTION_TYPE_TRADE
};

struct Trade {
	Item item1;
	int quantity1;

	Item item2;
	int quantity2;
};

struct Dialog{
	std::string text;
	char interactionType;

	std::vector<Trade*> trades;
};


class Dialogue {
public:
	Dialogue();

	void reset();
	void setDialogue(Dialog* d);

	void show();
	void hide();

	void update();
	void draw();

	bool isEngaged();

private:
	Sprite* dialogueBox, * arrowSelect;
	Sprite* dialogueTrade;
	Texture* dialog, * arrow;
	bool display;
	int selIndex;
	int selPos;
	int selFrame;
	int exitIndex;

	UI::UIText* tradeText;

	GameDialog* main;
	Dialog* info;
};

class DialogStack {
public:

	DialogStack(Dialogue* d);

	void addDialog(Dialog* d);
	void clearDialog();

	void update();

private:
	Dialogue* dial;
	std::queue<Dialog*> dialogs;

};