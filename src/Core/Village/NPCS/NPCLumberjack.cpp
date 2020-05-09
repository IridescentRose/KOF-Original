#include "NPCLumberjack.h"
#include "../../Inventory.h"
NPCLumber::NPCLumber(glm::vec2 startPosition, char radius, TilemapAnim* tmap1, TilemapAnim* tmap2, std::string file, std::string tag) : NPCBase(startPosition, radius, tmap1, tmap2, file, tag)
{
	movedIn = false;
	met = false;
}
#include "../Village.h"

Dialog* NPCLumber::getDialog()
{
	Dialog* d = new Dialog();

	d->interactionType = INTERACTION_TYPE_NONE;

	if (!movedIn && !met) {
		met = true;
		g_Inventory->tryAddItem(Items::LUMBERTABLE);
		d->text = "Lumber: Hello! I'm here to chop some trees and wood!\n I'll handle it automatically, and you can buy some of my goods\nfrom me with flesh. I'll give you my workshop\nblock, go build me a house and when you\nput it down I'll travel there.";
	}
	else if(!movedIn) {
		d->text = "Lumber: Is my house ready yet?\n";
	}
	else {
		
		d->text = "How can I help you?";
			d->interactionType = INTERACTION_TYPE_TRADE;
			d->trades.clear();

			Trade* t1 = new Trade();
			t1->item1 = Items::LOGS;
			t1->quantity1 = 1;
			t1->item2 = Items::GOLD;
			t1->quantity2 = 50;
			
			Trade* t2 = new Trade();
			t2->item1 = Items::GOLD;
			t2->quantity1 = 200;
			t2->item2 = Items::LOGS;
			t2->quantity2 = 1;
			
			Trade* t3 = new Trade();
			t3->item1 = Items::NONE;
			t3->quantity1 = 6;
			t3->item2 = Items::IRON_AXE;
			t3->quantity2 = 1;
			
			Trade* t4 = new Trade();
			t4->item1 = Items::GOLD;
			t4->quantity1 = 100;
			t4->item2 = Items::WOOD;
			t4->quantity2 = 1;
			
			d->trades.clear();
			d->trades.push_back(t1);
			d->trades.push_back(t2);
			d->trades.push_back(t3);
			d->trades.push_back(t4);
	}

	return d;
}
