#include "Enum.h"
#include <string>

#ifndef SKILL_H
#define SKILL_H

class Skill {
public:
    int value;
    int proficiency;

    // Default constructor
    Skill(int SkillType);

    // Getters
    int getAbilityType();
    int getSkillType();
	std::string getSkillName();

private:
    int abilityType;
    int skillType;
};

#endif