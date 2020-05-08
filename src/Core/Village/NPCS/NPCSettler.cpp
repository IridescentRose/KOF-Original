#include "NPCSettler.h"

NPCSettler::NPCSettler(glm::vec2 startPosition, char radius, TilemapAnim* tmap1, TilemapAnim* tmap2, std::string file, std::string tag) : NPCBase(startPosition, radius, tmap1, tmap2, file, tag)
{
}

Dialog* NPCSettler::getDialog()
{
	Dialog* d = new Dialog();

	d->interactionType = INTERACTION_TYPE_NONE;
	d->text = "I wish I was useful";


	return d;
}
