#include "headers/Skill.h"

// Getters
int Skill::getAbilityType() {
	return abilityType;
}
int Skill::getSkillType()
{
	return skillType;
}

std::string Skill::getSkillName()
{
	if (skillType == Athletics)
		return "Athletics";
	else if (skillType == Acrobatics)
		return "Acrobatics";
	else if (skillType == SleightOfHand)
		return "Sleight of Hand";
	else if (skillType == Stealth)
		return "Stealth";
	else if (skillType == Arcana)
		return "Arcana";
	else if (skillType == History)
		return "History";
	else if (skillType == Investigation)
		return "Investigation";
	else if (skillType == Nature)
		return "Nature";
	else if (skillType == Religion)
		return "Religion";
	else if (skillType == AnimalHandling)
		return "Animal Handling";
	else if (skillType == Insight)
		return "Insight";
	else if (skillType == Medicine)
		return "Medicine";
	else if (skillType == Perception)
		return "Perception";
	else if (skillType == Survival)
		return "Survival";
	else if (skillType == Deception)
		return "Deception";
	else if (skillType == Intimidation)
		return "Intimidation";
	else if (skillType == Performance)
		return "Performance";
	else if (skillType == Persuasion)
		return "Persuasion";
	else
		return "N/A";
}

// Default Constructor
Skill::Skill(int SkillType)
{
    skillType = SkillType;
	proficiency = 0;

	// Set proficiency type based on skill (enum is organized, so we use their indexes to determine)
    if (skillType == Athletics)
		abilityType = Strength;
    else if (skillType >= 1 && skillType <= 3)
		abilityType = Dexterity;
	else if (skillType >= 4 && skillType <= 8)
		abilityType = Intelligence;
	else if (skillType >= 9 && skillType <= 13)
		abilityType = Wisdom;
	else if (skillType >= 14)
		abilityType = Charisma;
}