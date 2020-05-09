#include "NPCMiner.h"
#include "../../Inventory.h"
NPCMiner::NPCMiner(glm::vec2 startPosition, char radius, TilemapAnim* tmap1, TilemapAnim* tmap2, std::string file, std::string tag) : NPCBase(startPosition, radius, tmap1, tmap2, file, tag)
{
	movedIn = false;
	met = false;
}
#include "../Village.h"

Dialog* NPCMiner::getDialog()
{
	Dialog* d = new Dialog();

	d->interactionType = INTERACTION_TYPE_NONE;

	if (!movedIn && !met) {
		met = true;
		g_Inventory->tryAddItem(Items::MINERTABLE);
		d->text = "Miner: Hey there. It's me again! I can excavate\nbelow the village and produce stone and ores automatically,\nand you can buy some of it from me (flesh of course).\n I'll give you my workshop block go build me a house and when you\nput it down I'll travel there.";
	}
	else if(!movedIn) {
		d->text = "Miner: Is my house ready yet?\n";
	}
	else {
			d->text = "How can I help you?";
			d->interactionType = INTERACTION_TYPE_TRADE;
			d->trades.clear();

			Trade* t1 = new Trade();
			t1->item1 = Items::STONES;
			t1->quantity1 = 1;
			t1->item2 = Items::GOLD;
			t1->quantity2 = 50;
			
			Trade* t2 = new Trade();
			t2->item1 = Items::GOLD;
			t2->quantity1 = 200;
			t2->item2 = Items::COAL;
			t2->quantity2 = 1;
			
			Trade* t3 = new Trade();
			t3->item1 = Items::NONE;
			t3->quantity1 = 6;
			t3->item2 = Items::IRON_PICKAXE;
			t3->quantity2 = 1;
			
			Trade* t4 = new Trade();
			t4->item1 = Items::NONE;
			t4->quantity1 = 2;
			t4->item2 = Items::STONES;
			t4->quantity2 = 20;
			
			d->trades.clear();
			d->trades.push_back(t1);
			d->trades.push_back(t2);
			d->trades.push_back(t3);
			d->trades.push_back(t4);
	}

	return d;
}
