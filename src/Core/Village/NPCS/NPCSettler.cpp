#include "NPCSettler.h"

NPCSettler::NPCSettler(glm::vec2 startPosition, char radius, TilemapAnim* tmap1, TilemapAnim* tmap2, std::string file, std::string tag) : NPCBase(startPosition, radius, tmap1, tmap2, file, tag)
{
	
}

#include "../../Inventory.h"

Dialog* NPCSettler::getDialog()
{
	Dialog* d = new Dialog();

	d->interactionType = INTERACTION_TYPE_NONE;

	if (!guide.playerHouseSetup) {
		guide.playerHouseSetup = true;
		g_Inventory->tryAddItem(Items::BED);
		g_Inventory->tryAddItem(Items::WORKBENCH);
		g_Inventory->tryAddItem(Items::CHEST);
		d->text = "Settler: Welcome to your settlement!\nI've placed a bed, workbench, and chest into\nyour inventory for you to go set up.\nGo build yourself a house, and come back when done.\nMake sure to place your bed - it is your respawn point.";
	}
	else if (!guide.playerHouseSetupComplete) {
		d->text = "Settler: Well, get on with it!\nRemember - your bed is your respawn point!";
	}
	else if (!guide.playerFarmSetup) {
		g_Inventory->tryAddItem(Items::IRON_SHOVEL);
		guide.playerFarmSetup = true;
		d->text = "Settler: Hey there! I see you set up\nyour base of operations! Now that you're all\nset let's start working on the village.\nThe first thing we need is going to be a farm for food.\nI've given you a shovel. Press X to turn grass to dirt.\nThen use your hoe to make farmland and plant seeds.";
	}
	else if (!guide.playerFarmSetupComplete) {
		d->text = "Settler: Your town needs food for\nit to grow and expand. Make sure you always have\nenough food for every villager!";
	}
	else if(!guide.playerNextCharacter) {
		d->text = "Settler: Every day, if your town can\nsupport it a new villager will appear here\nyou'll need to build them a house for\nprotection from raiders. After a while, raiders will\nbegin raiding at night time! We have to wait for now.";
	}
	else {
		d->text = "DEFAULT TEXT";
	}



	return d;
}

SettlerGuides guide = { 0, 0, 0, 0, 0, 0, 0};