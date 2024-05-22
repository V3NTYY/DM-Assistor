#ifndef ENUM_H
#define ENUM_H

#include "map"
#include <string>

enum StatType {
    Strength = 0,
    Dexterity,
    Constitution,
    Intelligence,
    Wisdom,
    Charisma
};

enum ConditionType {
	Blinded = 1,
	Charmed,
	Deafened,
	Frightened,
	Grappled,
	Incapacitated,
	Invisible,
	Paralyzed,
	Petrified,
	Poisoned,
	Prone,
	Restrained,
	Stunned,
	Unconscious
};

enum SkillType {
    // Strength-based skills
    Athletics = 0,

    // Dexterity-based skills
    Acrobatics,
    SleightOfHand,
    Stealth,

    // Intelligence-based skills
    Arcana,
    History,
    Investigation,
    Nature,
    Religion,

    // Wisdom-based skills
    AnimalHandling,
    Insight,
    Medicine,
    Perception,
    Survival,

    // Charisma-based skills
    Deception,
    Intimidation,
    Performance,
    Persuasion
};


enum Profiency {
    None = 0,
	Proficiency,
	Expertise
};

#endif