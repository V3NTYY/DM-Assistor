#include "Enum.h"
#include "Skill.h"
#include <vector>
#include "SpellBook.h".
#include "Condition.h"

#ifndef STAT_H
#define STAT_H

// Create a class for all of these typedefs, replace "int" with the class name
typedef int Feat;
typedef int Race;

class Stat {
public:
    Stat();
	void initConditions();

	/// Use the methods to apply changes to a statblock
	void updateScore(int score, int newValue);
	void updateSaveProf(int save, bool newValue);
	void updateSkillProf(int skill, int newValue);
	void updateHealth(int newValue);
	void updateMaxHealth(int newValue);
	void updateAC(int newValue);
	void updateSpeed(int newValue);
	bool toggleCondition(int newValue);

	/// Use the methods to directly access stats
	Skill getSkill(int skill);
	SpellBook& getSpellBook();
	int getPassivePerception();
	int getPassiveInvestigation();
	std::vector<int> getScoreMods();
	std::vector<int> getSaveMods();
	int getAC();
	int getSpeed();
	int getHealth();
	int getMaxHealth();
	std::vector<Condition> getActiveConditions();
	std::string returnSkills();
	std::string returnSaves();

	/// Method used to update all calculated stats
	void updateModifiables();

private:

	/// Hard-selected stats
	int StrScore, DexScore, ConScore, IntScore, WisScore, ChaScore; // Ability Scores
	bool StrSaveProf, DexSaveProf, ConSaveProf, IntSaveProf, WisSaveProf, ChaSaveProf; // Saving Throw Proficiencies
	int AC;
	int HP, MaxHP;
	int Speed;
	SpellBook SpellList; // All spells and spell slot charges

	/// Calculated stats
	std::vector<Skill> Skills; // Skills
	std::vector<Condition> Conditions; // Conditions
	int StrMod, DexMod, ConMod, IntMod, WisMod, ChaMod; // Ability Score Modifiers
	int StrSave, DexSave, ConSave, IntSave, WisSave, ChaSave; // Saving Throws
	int ProfBonus;
	int passivePerception, passiveInvestigation; // Passive Skills
};

#endif
