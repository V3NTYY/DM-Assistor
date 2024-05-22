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

	/// Use the methods to apply changes to a statblock
	void updateScore(int score, int newValue);
	void updateSaveProf(int save, bool newValue);
	void updateSkillProf(int skill, int newValue);
	void updateHealth(int newValue);
	void updateMaxHealth(int newValue);
	void updateAC(int newValue);
	void updateSpeed(int newValue);

	/// Use the methods to directly access stats
	Skill getSkill(int skill);
	int getPassivePerception();
	int getPassiveInvestigation();
	std::vector<int> getScoreMods();
	std::vector<int> getSaveMods();
	int getAC();
	int getSpeed();
	int getHealth();
	int getMaxHealth();
	std::string returnSkills();
	std::string returnSaves();

	/// Method used to update all calculated stats
	void updateModifiables();

	std::vector<Condition> Conditions; // Conditions
	std::vector<Feat> Feats; // Feats
	SpellBook Spells; // All spells and spell slot charges

private:

	/// Hard-selected stats
	int StrScore, DexScore, ConScore, IntScore, WisScore, ChaScore; // Ability Scores
	bool StrSaveProf, DexSaveProf, ConSaveProf, IntSaveProf, WisSaveProf, ChaSaveProf; // Saving Throw Proficiencies
	int AC;
	int HP, MaxHP;
	int Speed;

	/// Calculated stats
	std::vector<Skill> Skills; // Skills
	int StrMod, DexMod, ConMod, IntMod, WisMod, ChaMod; // Ability Score Modifiers
	int StrSave, DexSave, ConSave, IntSave, WisSave, ChaSave; // Saving Throws
	int ProfBonus;
	int passivePerception, passiveInvestigation; // Passive Skills
};

#endif
