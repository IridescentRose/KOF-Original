#include "Dialogue.h"
#include <Utilities/Input.h>
#include <Utilities/Logger.h>
Dialogue::Dialogue()
{
	dialog = TextureUtil::LoadPng("./assets/game/dialog.png");
	arrow = TextureUtil::LoadPng("./assets/game/utils.png");

	dialogueBox = new Sprite(dialog);
	arrowSelect = new Sprite(arrow, 16, 8, 8, 8, true);
	arrowSelect->SetPosition(240, 136);
	arrowSelect->Scale(2.0f, 2.0f);

	main = new GameDialog("example");
	main->reset();

	main->setOptions({ 0.5f, 0xFF000000, INTRAFONT_ALIGN_LEFT });
	main->setPosition({48, 184});
	main->setSpeed(1);

	dialogueBox->SetPosition(240, 200);
	dialogueBox->Scale(14.0f, 2.0f);

	dialogueTrade = new Sprite(dialog);
	dialogueTrade->SetPosition(360, 100);
	dialogueTrade->Scale(6.0f, 6.0f);

	tradeText = new UI::UIText({ 432, 32 }, "EXIT");
	tradeText->setOptions({ 0.5f, 0xFF000000, INTRAFONT_ALIGN_RIGHT });
	selPos = 0;
	display = false;
}

void Dialogue::reset()
{
	main->reset();
	display = false;
}

void Dialogue::setDialogue(Dialog* d)
{
	info = d;
	main->setText(d->text);
	selIndex = 0;
	selPos = 0;
	
	reset();
}

void Dialogue::show()
{
	display = true;
}

void Dialogue::hide()
{
	display = false;
}

#include "Inventory.h"
void Dialogue::update()
{
	if (display) {
		main->animateStep();

		if (main->isDone()) {
			switch (info->interactionType) {
			case INTERACTION_TYPE_NONE: {
				if (Utilities::KeyPressed(PSP_CTRL_CROSS)) {
					hide();
				}
				break;
			}

			case INTERACTION_TYPE_TRADE: {

				if (Utilities::KeyPressed(PSP_CTRL_CROSS)) {
					//1.) Find that we have the base resource

					for (int i = 0; i < 40; i++) {
						if (g_Inventory->getItemSlot(i)->item.ID == info->trades[selPos]->item1.ID) {
							//Okay we found it - do we have enough?
							if (g_Inventory->getItemSlot(i)->quantity >= info->trades[selPos]->quantity1) {
								//We have enough - let's trade and exit

								bool canAdd = g_Inventory->tryAddItem(info->trades[selPos]->item2);
								if (canAdd) {
									g_Inventory->getItemSlot(i)->quantity -= info->trades[selPos]->quantity1;

									if (g_Inventory->getItemSlot(i)->quantity == 0) {
										g_Inventory->getItemSlot(i)->item = Items::NONE;
									}

									for (int i = 0; i < info->trades[selPos]->quantity2 - 1; i++) {
										g_Inventory->tryAddItem(info->trades[selPos]->item2);
									}
								}

								break;
							}
						}
					}

				}

				if (Utilities::KeyPressed(PSP_CTRL_CIRCLE)) {
					hide();
				}

				if (Utilities::KeyPressed(PSP_CTRL_DOWN)) {
						
					selPos++;
					
					if (selPos > 3) {
						selPos = 3;
					}
				}
				if (Utilities::KeyPressed(PSP_CTRL_UP)) {
					selPos--;
					if (selPos < 0) {
						selPos = 0;
					}
				}
				break;
			}
			}

		}
	}
}

void Dialogue::draw()
{
	if (display) {
		dialogueBox->Draw();
		main->draw();

		if (info->interactionType == INTERACTION_TYPE_TRADE) {

			dialogueTrade->Draw();
			for (int i = selIndex; i < info->trades.size(); i++) {

				tradeText->setPosition({ 432, 32 + i * 32 });
				tradeText->setContent(std::to_string(info->trades[i]->quantity1) + " " + info->trades[i]->item1.name + " -> " + std::to_string(info->trades[i]->quantity2) + " " + info->trades[i]->item2.name);
				tradeText->draw();
			}

			arrowSelect->SetPosition(432, 32 - 12 + 32 * selPos);
			arrowSelect->Draw();
			
		}
	}
}

bool Dialogue::isEngaged()
{
	return display;
}

DialogStack::DialogStack(Dialogue* d)
{
	dial = d;
}

void DialogStack::addDialog(Dialog* d)
{
	dialogs.push(d);
}

void DialogStack::clearDialog()
{
	for (int i = 0; i < dialogs.size(); i++) {
		dialogs.pop();
	}
}

void DialogStack::update()
{
	if (!dial->isEngaged()) {
		if (dialogs.size() > 0) {
			dial->setDialogue(dialogs.front());
			dial->show();
			dialogs.pop();
		}
	}
}
