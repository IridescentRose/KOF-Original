#pragma once

#include <Graphics/UI/UIText.h>
#include <Utilities/Logger.h>
#include <map>

using namespace Stardust;
using namespace Stardust::Graphics;
using namespace Stardust::Graphics::UI;

struct CombatTextDetails{
	std::string text;
	int color;
	int ticks;
	glm::vec2 pos;
};

class CombatText {
public:
	CombatText();

	void addText(CombatTextDetails* d);
	void update();
	void draw();

private:
	std::map<int, CombatTextDetails*> map;
	UIText* txt;
};