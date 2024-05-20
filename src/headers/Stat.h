#include "Enum.h"
#include "Skill.h"
#include <vector>

#ifndef STAT_H
#define STAT_H

// Create a class for all of these typedefs, replace "int" with the class name
typedef int Condition;
typedef int Feat;
typedef int SpellBook;
typedef int Race;

class Stat {
public:
    Stat();
	std::string returnSkills();
	void updateModifiables();

	int StrScore, DexScore, ConScore, IntScore, WisScore, ChaScore; // Ability Scores
	bool StrSaveProf, DexSaveProf, ConSaveProf, IntSaveProf, WisSaveProf, ChaSaveProf; // Saving Throw Proficiencies
	int AC;
	int HP, MaxHP;
	int Speed;

	std::vector<Condition> Conditions; // Conditions
	std::vector<Feat> Feats; // Feats
	SpellBook Spells; // All spells and spell slot charges
	std::vector<Skill> Skills; // Skills

private:

	int StrMod, DexMod, ConMod, IntMod, WisMod, ChaMod; // Ability Score Modifiers
	int StrSave, DexSave, ConSave, IntSave, WisSave, ChaSave; // Saving Throws
	int ProfBonus;

	int PassiveInvestigation;
	int PassivePerception;
};

#endif
