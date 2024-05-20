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

