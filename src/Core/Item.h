#pragma once
#include <Graphics/2D/SpriteBase.h>
#include <Utilities/Logger.h>

using namespace Stardust;
using namespace Stardust::Graphics::Render2D;
using namespace Stardust::Graphics;


class Item;
class Items;
class ItemSprites;
class ItemDrop;
class DropManager;

enum ItemSpecials {
	ITEM_SPECIAL_SWORD,
	ITEM_SPECIAL_PICKAXE,
	ITEM_SPECIAL_SHOVEL,
	ITEM_SPECIAL_HOE,
	ITEM_SPECIAL_AXE
};

struct Item{
	char ID;
	bool isPlaceable;
	bool special;
	char specialType;
};

class Items {
public:
	static Item NONE;
	static Item IRON_SWORD;
	static Item IRON_PICKAXE;
	static Item IRON_HOE;
	static Item IRON_AXE;
	static Item IRON_SHOVEL;
	static Item WHEAT;
	static Item BREAD;
	static Item APPLE;
	static Item STONES;
	static Item COAL;
	static Item IRON_ORE;
	static Item GOLD_ORE;
	static Item RUBY_ORE;
	static Item AMETHYST_ORE;
	static Item DIAMOND_ORE;
	static Item GOLDEN_APPLE;
	static Item LOGS;
	static Item WOOD;
	static Item COBBLESTONE;
	static Item ACORN;
	static Item SEEDS;
	static Item VILLAGESPAWN;
	static Item WORKBENCH;
	static Item CHEST;
	static Item BED;
};

class ItemSprites {
public:
	ItemSprites();

	Sprite* getSprite(int idx);

private:
	Texture* items;
	Sprite* arr[25];
};

struct ItemDrop {
	Item itm;
	glm::vec2 pos;
	char quantity;
};

class DropManager {
public:
	DropManager();

	void addDrop(ItemDrop* drop);
	void clearDrops();

	void update(glm::vec2 position);
	void draw();

private:
	std::vector<ItemDrop*> drops;
};