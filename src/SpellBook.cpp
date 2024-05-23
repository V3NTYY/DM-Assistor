#include "headers/SpellBook.h"

SpellBook::SpellBook()
{
	// Initialize cantrips
	levelledSlots[0][0] = 0;
	levelledSlots[0][1] = 0;

	// Initialize levelled slots
	for (int i = 1; i < 10; i++)
    {
		levelledSlots[i][0] = 0;
		levelledSlots[i][1] = 0;
	}

	// Initialize total learned spells
	for (int i = 0; i < 10; i++)
		totalSpells[i] = std::vector<Spell>();
}

bool SpellBook::modifySlot(int slot, int newVal)
{
	if (slot < 1 || slot > 9)
		return false;

	levelledSlots[slot][0] = newVal;
	return true;
}

bool SpellBook::modifySlotMax(int slot, int newVal)
{
	if (slot < 1 || slot > 9)
		return false;

	levelledSlots[slot][1] = newVal;
	return true;
}

bool SpellBook::addSpell(Spell spell)
{
	int level = spell.getLevel();

	if (totalSpells[level].size() >= levelledSlots[level][1])
		return false;

	totalSpells[level].push_back(spell);
	return true;
}

bool SpellBook::removeSpell(Spell spell)
{
	int level = spell.getLevel();

	for (int i = 0; i < totalSpells[level].size(); i++)
	{
		if (totalSpells[level][i].getName() == spell.getName())
		{
			totalSpells[level].erase(totalSpells[level].begin() + i);
			return true;
		}
	}

	return false;
}

bool SpellBook::expendSlot(int slot)
{
	if ((slot < 1 || slot > 9) || levelledSlots[slot][0] <= 0)
		return false;

	levelledSlots[slot][0]--;

	return true;
}

bool SpellBook::replenishSlot(int slot)
{
	if ((slot < 1 || slot > 9) || levelledSlots[slot][0] >= levelledSlots[slot][1])
		return false;

	levelledSlots[slot][0]++;

	return true;
}

std::string SpellBook::printBook()
{
	std::string output = "";

	for (int i = 0; i < 10; i++)
	{
		if (totalSpells[i].size() > 0)
		{
			output += "Level " + std::to_string(i) + ": [" + std::to_string(levelledSlots[i][0]) + "," + std::to_string(levelledSlots[i][1]) + "]\n";
			for (int j = 0; j < totalSpells[i].size(); j++)
			{
				output += totalSpells[i][j].getName() + ": " + totalSpells[i][j].getDescription() + "\n";
			}
		}
	}

	return output;
}
