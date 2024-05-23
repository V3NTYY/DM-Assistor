#ifndef SPELLBOOK_H
#define SPELLBOOK_H

#include "Enum.h"
#include "headers/Spell.h"
#include <vector>

class SpellBook
{
public:
	SpellBook();
	bool modifySlot(int slot, int newVal);
	bool modifySlotMax(int slot, int newVal);
	bool addSpell(Spell spell);
	bool removeSpell(Spell spell);

	bool expendSlot(int slot);
	bool replenishSlot(int slot);
	std::string printBook();

private:

	std::vector<Spell> totalSpells[10];
	int levelledSlots[10][2];
};

#endif