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
		}
	}

	return d;
}
