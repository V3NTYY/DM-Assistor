#include "headers/Stat.h"

Stat::Stat()
{
	StrScore = 10;
	DexScore = 10;
	ConScore = 10;
	IntScore = 10;
	WisScore = 10;
	ChaScore = 10;

	AC = 10;
	HP = 10;
	MaxHP = 10;
	Speed = 30;
	ProfBonus = 2;

	StrSaveProf = false;
	DexSaveProf = false;
	ConSaveProf = false;
	IntSaveProf = false;
	WisSaveProf = false;
	ChaSaveProf = false;

	// Populate skill list
	for (int i = 0; i <= 17; i++)
		Skills.emplace_back(Skill(i));

	updateModifiables();
}

void Stat::updateModifiables()
{
	// Base Modifiers
	StrMod = (StrScore - 10) / 2;
	DexMod = (DexScore - 10) / 2;
	ConMod = (ConScore - 10) / 2;
	IntMod = (IntScore - 10) / 2;
	WisMod = (WisScore - 10) / 2;
	ChaMod = (ChaScore - 10) / 2;

	// Saving Throws
	StrSave = StrMod;
	DexSave = DexMod;
	ConSave = ConMod;
	IntSave = IntMod;
	WisSave = WisMod;
	ChaSave = ChaMod;

	// Saving Throw Proficiencies
	if (StrSaveProf)
		StrSave += ProfBonus;
	if (DexSaveProf)
		DexSave += ProfBonus;
	if (ConSaveProf)
		ConSave += ProfBonus;
	if (IntSaveProf)
		IntSave += ProfBonus;
	if (WisSaveProf)
		WisSave += ProfBonus;
	if (ChaSaveProf)
		ChaSave += ProfBonus;

	// Skill Proficiencies
	for (Skill& skill : Skills) {
		int abilityType = skill.getAbilityType();

		if (abilityType == Strength)
			skill.value = StrMod;
		else if (abilityType == Dexterity)
			skill.value = DexMod;
		else if (abilityType == Constitution)
			skill.value = ConMod;
		else if (abilityType == Intelligence)
			skill.value = IntMod;
		else if (abilityType == Wisdom)
			skill.value = WisMod;
		else if (abilityType == Charisma)
			skill.value = ChaMod;

		if (skill.proficiency == Proficiency)
			skill.value += ProfBonus;
		else if (skill.proficiency == Expertise)
			skill.value += ProfBonus * 2;
	}

	// Passive skills
	passivePerception = 10 + Skills[Perception].value;
	passiveInvestigation = 10 + Skills[Investigation].value;
}

std::string Stat::returnSkills()
{
	std::string skillList = "";
	skillList.append("Skills:\n");
	for (Skill& skill : Skills) {
		skillList.append(skill.getSkillName() + ": ");
		if (skill.value >= 0)
			skillList.append("+" + std::to_string(skill.value) + "\n");
		else
			skillList.append(std::to_string(skill.value) + "\n");
	}

	return skillList;
}

std::string Stat::returnSaves()
{
	std::string saveList = "";
	saveList.append("Saves:\n");
	
	saveList.append("Strength Save: " + std::to_string(StrSave) + "\n");
	saveList.append("Dexterity Save: " + std::to_string(DexSave) + "\n");
	saveList.append("Constitution Save: " + std::to_string(ConSave) + "\n");
	saveList.append("Intelligence Save: " + std::to_string(IntSave) + "\n");
	saveList.append("Wisdom Save: " + std::to_string(WisSave) + "\n");
	saveList.append("Charisma Save: " + std::to_string(ChaSave) + "\n");

	return saveList;
}

void Stat::updateScore(int score, int newValue)
{
	switch (score)
	{
	case Strength:
		StrScore = newValue;
		break;
	case Dexterity:
		DexScore = newValue;
		break;
	case Constitution:
		ConScore = newValue;
		break;
	case Intelligence:
		IntScore = newValue;
		break;
	case Wisdom:
		WisScore = newValue;
		break;
	case Charisma:
		ChaScore = newValue;
		break;
	}

	updateModifiables();
}

void Stat::updateSaveProf(int save, bool newValue)
{
	switch (save)
	{
	case Strength:
		StrSaveProf = newValue;
		break;
	case Dexterity:
		DexSaveProf = newValue;
		break;
	case Constitution:
		ConSaveProf = newValue;
		break;
	case Intelligence:
		IntSaveProf = newValue;
		break;
	case Wisdom:
		WisSaveProf = newValue;
		break;
	case Charisma:
		ChaSaveProf = newValue;
	}

	updateModifiables();
}

void Stat::updateSkillProf(int skill, int newValue)
{
	if (skill >= Skills.size())
		return;

	Skills[skill].proficiency = newValue;
	updateModifiables();
}

void Stat::updateHealth(int newValue)
{
	HP = newValue;
	if (HP > MaxHP)
		HP = MaxHP;
	else if (HP < 0)
		HP = 0;
}

void Stat::updateMaxHealth(int newValue)
{
	MaxHP = newValue;
	if (MaxHP < 0)
		MaxHP = 1;

	if (MaxHP < HP)
		updateHealth(MaxHP);
}

void Stat::updateAC(int newValue)
{
	AC = newValue;

	if (AC < 0)
		AC = 0;
}

void Stat::updateSpeed(int newValue)
{
	Speed = newValue;
	if (Speed < 0)
		Speed = 0;
}

Skill Stat::getSkill(int skill)
{
	if (skill >= Skills.size())
		return Skills[0];

	return Skills[skill];
}

int Stat::getPassivePerception()
{
	return passivePerception;
}

int Stat::getPassiveInvestigation()
{
	return passiveInvestigation;
}

std::vector<int> Stat::getScoreMods()
{
	std::vector<int> scores = { StrScore, DexScore, ConScore, IntScore, WisScore, ChaScore };
	return scores;
}

std::vector<int> Stat::getSaveMods()
{
	std::vector<int> scores = { StrSave, DexSave, ConSave, IntSave, WisSave, ChaSave };
	return scores;
}

int Stat::getAC()
{
	return AC;
}

int Stat::getSpeed()
{
	return Speed;
}

int Stat::getHealth()
{
	return HP;
}

int Stat::getMaxHealth()
{
	return MaxHP;
}

