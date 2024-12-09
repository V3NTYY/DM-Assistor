#ifndef DICEROLLER_H
#define DICEROLLER_H
#include <string>

class DiceRoller
{
public:
	DiceRoller() : goodRolls(0), badRolls(0), karmicDice(false) {}
	void toggleKarmicDice();
	int roll(int sides, int modifier, bool advantage, bool disadvantage);
	std::string debugPrint();

private:
	bool karmicDice;
	int badRolls;
	int goodRolls;
};

#endif