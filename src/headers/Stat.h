#ifndef STAT_H
#define STAT_H

#include "Enum.h"
#include "Skill.h"
#include "Feature.h"
#include "SpellBook.h".
#include "Condition.h"
#include <vector>

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

	/// Feature removal/addition
	bool addFeat(Feature f);
	bool removeFeat(Feature f);

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
	std::vector<Feature> getFeatures();
	std::string returnSkills();
	std::string returnSaves();

	/// Method used to update all calculated stats
	void updateModifiables(bool updateFeats);

private:

	/// Hard-selected stats
	int StrScore, DexScore, ConScore, IntScore, WisScore, ChaScore; // Ability Scores
	bool StrSaveProf, DexSaveProf, ConSaveProf, IntSaveProf, WisSaveProf, ChaSaveProf; // Saving Throw Proficiencies
	int AC;
	int HP, MaxHP;
	int Speed;
	SpellBook SpellList; // All spells and spell slot charges
	std::vector<Condition> Conditions; // Conditions
	std::vector<Feature> Features; // Features

	/// Calculated stats
	std::vector<Skill> Skills; // Skills
	int StrMod, DexMod, ConMod, IntMod, WisMod, ChaMod; // Ability Score Modifiers
	int StrSave, DexSave, ConSave, IntSave, WisSave, ChaSave; // Saving Throws
	int ProfBonus;
	int passivePerception, passiveInvestigation; // Passive Skills
};

#endif
