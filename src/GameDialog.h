#pragma once
#include <Graphics/UI/UIText.h>


using namespace Stardust::Graphics;

class GameDialog {
public:
	GameDialog(std::string text);
	~GameDialog();

	void setPosition(glm::vec2 position);
	void setSpeed(int s);

	void setText(std::string);
	void reset();

	void animateStep();
	void draw();

	void setOptions(UI::FontStyle fs);

private:

	UI::UIText* ui;
	
	std::string txt;
	int index;
	int speed;

};