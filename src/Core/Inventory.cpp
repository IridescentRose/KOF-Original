#include "Inventory.h"
#include <Utilities/Input.h>

Inventory::Inventory()
{
	util = TextureUtil::LoadPng("./assets/game/utils.png");
	hotbarSlot = new Sprite(util, 0, 8, 8, 8, true);
	hotbarSelector = new Sprite(util, 8, 8, 8, 8, true);
	hotbarSlot->Scale(4.0f, 4.0f);
	hotbarSelector->Scale(4.0f, 4.0f);
	selPos = 0;

	inv = TextureUtil::LoadPng("./assets/game/inventory.png");
	inventory = new Sprite(inv);
	inventory->Scale(4.0f, 4.0f);
	inventory->SetPosition(240, 136);

	arrow = new Sprite(util, 32, 8, 8, 8, true);
	arrow->Scale(2.0f, 2.0f);
	arrow->SetPosition(240, 136);

	position = { 0, 0 };

	for (int i = 0; i < 40; i++) {
		slots[i] = { Items::NONE, 0 };
	}
	sprites = new ItemSprites();

	slots[0] = { Items::IRON_SWORD, 1 };
	slots[1] = { Items::BREAD, 3 };
	slots[11] = { Items::BREAD, 3 };

	txt = new UIText({0, 0}, "");
	txt->setOptions({ 0.5f, 0xFFFFFFFF, INTRAFONT_ALIGN_RIGHT });

	engaged = false;
	held = { Items::NONE, 0 };
}

void Inventory::setHotbarSelect(int i)
{
	selPos = i;
}

void Inventory::drawHotbar()
{
	for (int i = 0; i < 10; i++) {
		hotbarSlot->SetPosition(80 + i * 32, 272 - 34);
		hotbarSlot->Draw();
	}

	for (int i = 0; i < 10; i++) {
		if (slots[i].item.ID != Items::NONE.ID) {
			auto spr = sprites->getSprite(slots[i].item.ID);
			spr->SetPosition(88 + i * 32, 272 - 26);
			spr->Draw();

			if (!slots[i].item.special && slots[i].quantity > 1) {
				txt->setContent(std::to_string((int)slots[i].quantity));
				txt->setPosition({ 110 + i * 32, 272 - 6 });
				txt->draw();
			}

		}
	}

	hotbarSelector->SetPosition(80 + selPos * 32, 272 - 34);
	hotbarSelector->Draw();
}

void Inventory::show()
{
	engaged = true;
}

void Inventory::hide()
{
	engaged = false;
}

bool Inventory::tryAddItem(Item itm)
{
	int idx = findItem(itm);
	if (idx > 0 && !itm.special) {
		slots[idx].quantity++;
		return true;
	}
	else {
		int idx = findItem(Items::NONE);
		if (idx > 0) {
			slots[idx].item = itm;
			slots[idx].quantity = 1;
			return true;
		}
	}
	return false;
}

int Inventory::findItem(Item itm)
{
	int idx = -1;

	for (int i = 0; i < 40; i++) {
		if (itm.ID == slots[i].item.ID) {
			return i;
		}
	}

	return idx;
}

bool Inventory::isEngaged()
{
	return engaged;
}

void Inventory::update()
{
	if (engaged) {
		if (Utilities::KeyPressed(PSP_CTRL_DOWN)) {
			position.y--;
		}
		if (Utilities::KeyPressed(PSP_CTRL_UP)) {
			position.y++;
		}

		if (position.y > 3) {
			position.y = 3;
		}
		if (position.y < 0) {
			position.y = 0;
		}

		if (Utilities::KeyPressed(PSP_CTRL_LEFT)) {
			position.x--;
		}
		if (Utilities::KeyPressed(PSP_CTRL_RIGHT)) {
			position.x++;
		}

		if (position.x > 9) {
			position.x = 9;
		}
		if (position.x < 0) {
			position.x = 0;
		}

		if (Utilities::KeyPressed(PSP_CTRL_CIRCLE)) {
			hide();
		}

		if (Utilities::KeyPressed(PSP_CTRL_CROSS)) {
			if (held.item.ID == Items::NONE.ID) {
				//We can pick up
				if (slots[position.y * 10 + position.x].item.ID != Items::NONE.ID) {
					//We know that our item slot has something in it.

					held = slots[position.y * 10 + position.x];
					slots[position.y * 10 + position.x] = { Items::NONE, 0 };
				}
			}
			else {
				if (slots[position.y * 10 + position.x].item.ID == Items::NONE.ID) {

					slots[position.y * 10 + position.x] = held;
					held = { Items::NONE, 0 };
				}
				else if (slots[position.y * 10 + position.x].item.ID == held.item.ID) {
					if (!held.item.special) {
						slots[position.y * 10 + position.x].quantity += held.quantity;
						held = { Items::NONE, 0 };
					}
				}
			}
		}

	}
}

void Inventory::draw()
{
	if (engaged) {
		inventory->Draw();

		for (int i = 10; i < 40; i++) {

			int x = i % 10;
			int y = i / 10;

			if (slots[i].item.ID != Items::NONE.ID) {
				auto spr = sprites->getSprite(slots[i].item.ID);
				spr->SetPosition(32 + x * 44, 272 - (72 + y * 36));
				spr->Draw();

				if (!slots[i].item.special && slots[i].quantity > 1) {
					txt->setContent(std::to_string((int)slots[i].quantity));
					txt->setPosition({ 52 + x * 44, 272 - (52 + y * 36) });
					txt->draw();
				}

			}
		}


		if (position.y > 0) {
			arrow->SetPosition(40 + position.x * 44, 272 - (64 + position.y * 36));
		}
		else {
			arrow->SetPosition(96 + position.x * 32, 272 - (18 + position.y * 36));
		}
		if (held.item.ID != Items::NONE.ID) {
			auto spr = sprites->getSprite(held.item.ID);
			if (position.y > 0) {
				spr->SetPosition(32 + position.x * 44, 272 - (72 + position.y * 36));
			}
			else {
				spr->SetPosition(86 + position.x * 32, 272 - (26 + position.y * 36));
			}
			spr->Draw();
		}
		arrow->Draw();
	}
}
