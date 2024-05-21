#include "headers/DiceRoller.h"
#include <stdlib.h>
#include <time.h>

void DiceRoller::toggleKarmicDice()
{
	karmicDice = !karmicDice;
	badRolls = 0;
	goodRolls = 0;
}

int DiceRoller::roll(int sides, int modifier, bool advantage, bool disadvantage)
{
	int total = 0;
	int firstRoll = 0;
	int secondRoll = 0;

	// Advantage and disadvantage cancel each other out
	if (advantage && disadvantage)
	{
		advantage = false;
		disadvantage = false;
	}

	firstRoll = rand() % sides + 1;
	secondRoll = rand() % sides + 1;

	total = firstRoll;

	if (advantage)
		total = firstRoll > secondRoll ? firstRoll : secondRoll;

	else if (disadvantage)
		total = firstRoll < secondRoll ? firstRoll : secondRoll;

	// Karmic dice options
	if (total < (sides / 2)) {
		goodRolls = 0;
		badRolls++;
	}
	else if (total > (sides / 2)) {
		badRolls = 0;
		goodRolls++;
	}

	if (karmicDice && (goodRolls >= 6 || badRolls >= 6)) {
		goodRolls = 0;
		badRolls = 0;
		return roll(sides, modifier, advantage, disadvantage);
	}

	return total + modifier;
}

std::string DiceRoller::debugPrint()
{
	return "Good Rolls: " + std::to_string(goodRolls) + " Bad Rolls: " + std::to_string(badRolls);
}
