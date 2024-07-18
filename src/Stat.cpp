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

	SpellList = SpellBook();

	initConditions();
	updateModifiables(true);
}

void Stat::initConditions()
{
	for (int i = 0; i <= 15; i++)
		Conditions.emplace_back(Condition("Null", "Null", false));

	Conditions[0] = Condition("Null", "Null", false);
	Conditions[Blinded] = Condition("Blinded", "A blinded creature can't see and automatically fails any ability check that requires sight.", false);
	Conditions[Charmed] = Condition("Charmed", "A charmed creature can't attack the charmer or target the charmer with harmful abilities or magical effects.", false);
	Conditions[Deafened] = Condition("Deafened", "A deafened creature can't hear and automatically fails any ability check that requires hearing.", false);
	Conditions[Frightened] = Condition("Frightened", "A frightened creature has disadvantage on ability checks and attack rolls while the source of its fear is within line of sight.", false);
	Conditions[Grappled] = Condition("Grappled", "A grappled creature's speed becomes 0, and it can't benefit from any bonus to its speed.", false);
	Conditions[Incapacitated] = Condition("Incapacitated", "An incapacitated creature can't take actions or reactions.", false);
	Conditions[Invisible] = Condition("Invisible", "An invisible creature is impossible to see without the aid of magic or a special sense. For the purpose of hiding, the creature is heavily obscured.", false);
	Conditions[Paralyzed] = Condition("Paralyzed", "A paralyzed creature is incapacitated and can't move or speak.", false);
	Conditions[Petrified] = Condition("Petrified", "A petrified creature is transformed, along with any nonmagical object it is wearing or carrying, into a solid inanimate substance (usually stone). Its weight increases by a factor of ten, and it ceases aging.", false);
	Conditions[Poisoned] = Condition("Poisoned", "A poisoned creature has disadvantage on attack rolls and ability checks.", false);
	Conditions[Prone] = Condition("Prone", "A prone creature's only movement option is to crawl, unless it stands up and thereby ends the condition.", false);
	Conditions[Restrained] = Condition("Restrained", "A restrained creature's speed becomes 0, and it can't benefit from any bonus to its speed.", false);
	Conditions[Stunned] = Condition("Stunned", "A stunned creature is incapacitated, can't move, and can speak only falteringly.", false);
	Conditions[Unconscious] = Condition("Unconscious", "An unconscious creature is incapacitated, can't move or speak, and is unaware of its surroundings.", false);
}

void Stat::updateModifiables(bool updateFeats)
{
	// Update feats using reference pointer to stat
	if (updateFeats) {
		for (Feature& feat : Features)
			feat.update(this);
	}

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
	initiativeMod = DexMod;
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

	updateModifiables(false);
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

	updateModifiables(false);
}

void Stat::updateSkillProf(int skill, int newValue)
{
	if (skill >= Skills.size())
		return;

	Skills[skill].proficiency = newValue;
	updateModifiables(false);
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

void Stat::updateInitative(int newValue)
{
	initiativeMod = newValue;
	if (initiativeMod < 0)
		initiativeMod = 0;
}

bool Stat::toggleCondition(int newValue)
{
	if (newValue >= Conditions.size() || newValue < 0)
		return false;

	if (Conditions[newValue].isActive())
		Conditions[newValue].setActive(false);
	else
		Conditions[newValue].setActive(true);
	return true;
}

bool Stat::addFeat(Feature f)
{
	// Ensure we have an actual feat
	if (f.name == "Null" && f.desc == "Null")
		return false;

	// Ensure we aren't adding a duplicate feat
	if (!f.repeatable) {
		for (Feature& feat : Features) {
			if (feat == f)
				return false;

			Feature* chainTemp = &feat;
			Feature* fChainTemp = &f;

			// Make sure we aren't adding a feat that is chained already
			while (chainTemp != nullptr) {
				if (*chainTemp == f)
					return false;
				chainTemp = chainTemp->getChain();
			}

			// Make sure we aren't adding a feat that CONTAINS a chain that exists already
			while (fChainTemp != nullptr) {
				if (*fChainTemp == feat)
					return false;
				fChainTemp = fChainTemp->getChain();
			}
		}
	}

	f.update(this);
	Features.push_back(f);
	return true;
}

bool Stat::removeFeat(Feature f)
{
	// Grab a pointer reference to the feature to be removed
	auto it = std::find_if(Features.begin(), Features.end(), [&f](const Feature& feat) { return feat == f; });

	// Feature not found
	if (it == Features.end())
		return false;

	// Remove the feature from the list
	it->remove(this);
	Features.erase(it);

	return true;

}

Skill Stat::getSkill(int skill)
{
	if (skill >= Skills.size())
		return Skills[0];

	return Skills[skill];
}

SpellBook& Stat::getSpellBook()
{
	return SpellList;
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

std::vector<Condition> Stat::getActiveConditions()
{
	std::vector<Condition> activeConditions;
	for (Condition& condition : Conditions)
		if (condition.isActive())
			activeConditions.push_back(condition);
	return activeConditions;
}

std::vector<Feature> Stat::getFeatures()
{
	return Features;
}

