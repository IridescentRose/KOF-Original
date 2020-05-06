#include "NumptyTutorial.h"
#include "../../States/Tutorial.h"

NumptyTutorial::NumptyTutorial(glm::vec2 startPosition, char r, Tilemap* tmap1, Tilemap* tmap2, std::string file, std::string t)
{
	position = startPosition;
	origin = startPosition;

	radius = r;
	ticksTilAction = rand() % 300;
	nextAction = NPC_ACTION_LOOK;

	tex = TextureUtil::LoadPng(file);
	characterModel = new CharacterSprite({ 32, 32 }, new TextureAtlas(4), tex);

	Render2D::CharacterDirectionalAnimInfo* idle = new Render2D::CharacterDirectionalAnimInfo();
	Render2D::CharacterDirectionalAnimInfo* walk = new Render2D::CharacterDirectionalAnimInfo();

	idle->down = { 1, 1 };
	idle->left = { 5, 1 };
	idle->right = { 9, 1 };
	idle->top = { 13,1 };


	walk->down = { 0, 4 };
	walk->left = { 4, 4 };
	walk->right = { 8, 4 };
	walk->top = { 12, 4 };


	characterModel->addAnimEvent("idle", idle);
	characterModel->addAnimEvent("walk", walk);
	characterModel->setDefaultAnimEvent("idle");
	characterModel->triggerAnimEvent("idle");
	characterModel->triggerAnimEvent("walk");
	characterModel->setFacing(Render2D::CHARACTER_FACING_DOWN);

	controller = new Render2D::TopDownController(characterModel, 128.0f, false);
	controller->getAnimController()->setCharacterTickRateRelative(24);
	controller->setPosition(position);

	controller->addPhysicsTileMap(tmap1);
	controller->addPhysicsTileMap(tmap2);
	ticksMovement = -1;

	movement = { 0, 0 };
	tag = t;
}

void NumptyTutorial::simpleAITick()
{
	if (tag == "numpty") {

		ticksTilAction--;
		ticksMovement--;

		if (ticksTilAction <= 0) {

			handleAction(nextAction);

			ticksTilAction = rand() % 300;
			nextAction = rand() % 2;
		}
	}
}


void NumptyTutorial::draw()
{
	controller->draw();
}

void NumptyTutorial::update()
{
	if (ticksMovement > 0) {
		controller->velocity = movement * 0.16f;
	}
	else {
		movement = { 0, 0 };
	}
	position = controller->getCharacterSprite()->getPosition();
	controller->update(0.16f);
}

void NumptyTutorial::handleAction(char action)
{
	ticksMovement = rand() % 15 + 5;

	if (action == NPC_ACTION_WALK) {
		switch (controller->getCharacterSprite()->getFacing()) {
		case CHARACTER_FACING_DOWN: {
			movement = { 0.f, 64.f };
			break;
		}
		case CHARACTER_FACING_UP: {
			movement = { 0.f, -64.f };
			break;
		}
		case CHARACTER_FACING_LEFT: {
			movement = { -64.f, 0.0f };
			break;
		}
		case CHARACTER_FACING_RIGHT: {
			movement = { 64.f, 0.0f };
			break;
		}
		}

		controller->getCharacterSprite()->triggerAnimEvent("walk");
	}
	else {
		char dir = rand() % 4;
		controller->getCharacterSprite()->setFacing(dir);
	}

}

Dialog* NumptyTutorial::getDialog()
{
	Dialog* d = new Dialog();

	d->interactionType = INTERACTION_TYPE_NONE;

	if (tag == "settler") {
		if (!progInfo.talkToSettler) {
			d->text = "Hello old friend. It's certainly been a while, hasn't it?\nI know you want to get going on our journey... but look,\nYou don't have any tools to get started! Go visit the\nfarmer, guard, miner, and lumberjack.They should\nbe able to give you some tools!. Meet me again when ready!";
			progInfo.talkToSettler = true;
		}
		else if(!(progInfo.completeFarmer && progInfo.completeGuard && progInfo.completeLumber && progInfo.completeMiner)){
			d->text = "You still need more tools!";
		}
		else {
			d->text = "Well... before we go... I need you to pay me 1000 gold.\n Why? I want to make sure this journey is worth my while!\nWhat's this? You don't have enough? Well I guess... I need\nalternative payment. 4 hearts and it's a deal!\nReady or not, here I come!";
			progInfo.tutorialCompleted = true;
		}
	}
	else if (tag == "farmer" && progInfo.talkToSettler) {
		if (!progInfo.talkFarmer) {
			progInfo.talkFarmer = true;
			d->text = "Morning to ya' whippersnapper! How have you been?\nSo why have you come to me today? I see... you need tools...\nI have a spare hoe fer you, and you can keep it if you\ngo harvest some wheat from my farm and bring it back!\nI'll even give ya' something worthwhile!";
			
			g_Inventory->tryAddItem(Items::IRON_HOE);
		}
		else if (progInfo.canCompleteFarmer) {
			progInfo.completeFarmer = true;
			d->text = "Thanks whippersnapper! Now let's get down to business.\nTake some bread here, and gold... and come here...\n*CHOMP*\nThank you fer the donation!";
		}
		else if (!progInfo.canCompleteFarmer) {
			d->text = "Get on with it, whippersnapper!";
		}
		else {
			d->text = "I hope to see ya' again whippersnapper! I can't forget the\ntaste!";
		}
	}
	else if (tag == "guard" && progInfo.talkToSettler) {
		if (!progInfo.talkGuard) {
			progInfo.talkGuard = true;
			d->text = "Good morning sir! What brings you here? I see...\nYou need a weapon to defend yourself! Let's see!\nI spotted some bandits to the north... I got one, but the\nother escaped! Take this sword and take him on.\nIf you succeed, keep it as a gift.";
			g_Inventory->tryAddItem(Items::IRON_SWORD);
		}
		else if (progInfo.canCompleteGuard) {
			progInfo.completeGuard = true;
			d->text = "Thank you! Here's the bounty!\n";
		}
		else if (!progInfo.canCompleteGuard) {
			d->text = "Have you found him yet? People are in danger!";
		}
		else {
			d->text = "It's a rough world out there. Stay safe.";
		}
	}
	else if (tag == "miner" && progInfo.talkToSettler) {
		if (!progInfo.talkMiner) {
			progInfo.talkMiner = true;
			d->text = "Hello. What brings you here? Ah...\nI can lend you a pickaxe, but you're going to need to get\nsome stone to help me replace it. I'll even pay you!";
			g_Inventory->tryAddItem(Items::IRON_PICKAXE);
		}
		else if (progInfo.canCompleteMiner) {
			progInfo.canCompleteMiner = true;
			d->text = "Ah! This is great. Just one more thing...\n*CHOMP* Delicious!";
		}
		else if (!progInfo.canCompleteMiner) {
			d->text = "These stones won't mine themselves!";
		}
		else {
			d->text = "Be as unbreakable as stone.";
		}
	}
	else if (tag == "lumberjack" && progInfo.talkToSettler) {
		if (!progInfo.talkLumber) {
			progInfo.talkLumber = true;
			d->text = "Mornin' to you, son. How can I help you? Hm...\nI can give you an old axe, but you'll need to go get\nme a few logs. I'll get you something special!";
			g_Inventory->tryAddItem(Items::IRON_AXE);
		}
		else if (progInfo.canCompleteLumber) {
			progInfo.canCompleteLumber = true;
			d->text = "Hello, hello! Thank you for the wood!\nAs promised, take this apple!";
		}
		else if (!progInfo.canCompleteLumber) {
			d->text = "I still see those trees! What are you doing?";
		}
		else {
			d->text = "I have a feeling you'll be seeing a lot of trees.";
		}
	}
	else {
		char r = rand() % 10;

		switch (r) {
		case 0: {
			d->text = "Hello. How are you? I hope your day has gone well so far.\nI'm fine. Couldn't be better!";
			break;
		}case 1: {
			d->text = "I wish I could have something to eat... mmmm flesh...\nOh! Excuse me. I was just... daydreaming...";
			break;
		}case 2: {
			d->text = "One day I'll be a great builder! It's my dream!";
			break;
		}case 3: {
			d->text = "Have you seen the lake? I wish I could sit back and fish!\nSadly I don't have a fishing rod...\nHe hasn't implemented that!";
			break;
		}case 4: {
			d->text = "This game's cool and all, but where is my MINECRAFT!1!111!\n";
			break;
		}case 5: {
			d->text = "I'd love to eat you. But then I'd have nothing to eat...";
			break;
		}case 6: {
			d->text = "Bread is nice and all, but where's that meat!\nSorry, I was thinking out loud...";
			break;
		}case 7: {
			d->text = "Have you seen the bandits? They look so scary!\nThankfully we have our guard. I don't wanna die again!";
			break;
		}case 8: {
			d->text = "I wonder if I'll ever get to see outside of this village.\nIf you move out... I'll find you again!";
			break;
		}case 9: {
			d->text = "Thank you. For what? Keeping me company.\n.............\nBut don't you have something to do?";
			break;
		}
		}
	}

	return d;
}
