#include "NPCFarmer.h"
#include "../../Inventory.h"
NPCFarmer::NPCFarmer(glm::vec2 startPosition, char radius, TilemapAnim* tmap1, TilemapAnim* tmap2, std::string file, std::string tag) : NPCBase(startPosition, radius, tmap1, tmap2, file, tag)
{
	movedIn = false;
	met = false;
}
#include "../Village.h"

Dialog* NPCFarmer::getDialog()
{
	Dialog* d = new Dialog();

	d->interactionType = INTERACTION_TYPE_NONE;

	if (!movedIn && !met) {
		met = true;
		g_Inventory->tryAddItem(Items::FARMERTABLE);
		d->text = "Farmer: Hiya there. How are you? I can manage\nyour crops and produce bread as long as you have 5\nwheat crops I'll handle it automatically, and you can\nbuy some of it from me with that flesh of yers'. I'll give\n you my workshop block go build me a house and when you\nput it down I'll travel there.";
	}
	else if(!movedIn) {
		d->text = "Farmer: Is my house ready yet?\n";
	}
	else {
		if (g_Village->farmlandTotal < 5) {
			d->text = "Farmer: We don't have enough farmland!\n";
		}
		else {
			d->text = "How can I help you?";
			d->interactionType = INTERACTION_TYPE_TRADE;
			d->trades.clear();

			Trade* t1 = new Trade();
			t1->item1 = Items::WHEAT;
			t1->quantity1 = 1;
			t1->item2 = Items::GOLD;
			t1->quantity2 = 50;
			
			Trade* t2 = new Trade();
			t2->item1 = Items::GOLD;
			t2->quantity1 = 200;
			t2->item2 = Items::BREAD;
			t2->quantity2 = 1;
			
			Trade* t3 = new Trade();
			t3->item1 = Items::NONE;
			t3->quantity1 = 4;
			t3->item2 = Items::IRON_HOE;
			t3->quantity2 = 1;
			
			Trade* t4 = new Trade();
			t4->item1 = Items::NONE;
			t4->quantity1 = 2;
			t4->item2 = Items::BREAD;
			t4->quantity2 = 1;
			
			d->trades.clear();
			d->trades.push_back(t1);
			d->trades.push_back(t2);
			d->trades.push_back(t3);
			d->trades.push_back(t4);
		}	
	}

	return d;
}
