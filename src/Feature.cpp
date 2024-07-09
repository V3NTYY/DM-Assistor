#include "headers/Feature.h"
#include "headers/Stat.h"
#include <wx/msgdlg.h>

Feature::Feature()
{
	name = "Null";
	desc = "Null";
	FeatType = NORMAL;
	chain = nullptr;
	applied = false;
	identifier = 0;

	for (int i = 0; i < 6; i++)
		AbilityScoreMod[i][0] = -1;

	for (int i = 0; i < 6; i++)
		SaveProfMod[i][0] = -1;

	for (int i = 0; i < 18; i++)
		SkillProfMod[i][0] = -1;

	MaxHPMod = -1;
	ACMod = -1;
	SpeedMod = -1;

}

void Feature::saveFeat(Feature f, std::string file)
{
	std::ofstream o(file);
	json j = {
		{"name", f.name},
		{"desc", f.desc},
		{"type", f.FeatType},
		{"abilityScoreMod", f.AbilityScoreMod},
		{"saveProfMod", f.SaveProfMod},
		{"skillProfMod", f.SkillProfMod},
		{"maxHPMod", f.MaxHPMod},
		{"ACMod", f.ACMod},
		{"SpeedMod", f.SpeedMod}
	};

	if (!o) {
		wxMessageBox("Failed to open/create JSON.", "Error", wxICON_ERROR | wxOK);
		return;
	}

	o << j.dump();

	if (!o)
		wxMessageBox("Failed to write feature to JSON.", "Error", wxICON_ERROR | wxOK);
}

Feature Feature::loadFeat(std::string file)
{
	Feature newFeat = Feature();

	std::ifstream f(file);
	// If we can't find the file, just return an ungenerated feat. addFeat() will handle the error.
	if (!f)
		return Feature();

	json data = json::parse(f);

	// Load data from JSON. If a field isn't found, it is set to Null or ignored (-1).
	std::string name = data.value("name", "Null");
	std::string desc = data.value("desc", "Null");
	int type = data.value("type", -1);
	int MaxHPMod = data.value("maxHPMod", -1);
	int ACMod = data.value("ACMod", -1);
	int SpeedMod = data.value("SpeedMod", -1);

	int AbilityScoreMod[6][1] = { {-1} };
	int SaveProfMod[6][1] = { {-1} };
	int SkillProfMod[18][1] = { {-1} };

	/// Handle the arrays from JSON
	auto it = data.find("abilityScoreMod");
	if (it != data.end() && it->is_array()) {
		int i = 0;
		for (auto& item : *it) {
			if (i < 6)
				AbilityScoreMod[i][0] = item[0].get<int>();
			i++;
		}
	}

	it = data.find("saveProfMod");
	if (it != data.end() && it->is_array()) {
		int i = 0;
		for (auto& item : *it) {
			if (i < 6)
				SaveProfMod[i][0] = item[0].get<int>();
			i++;
		}
	}

	it = data.find("skillProfMod");
	if (it != data.end() && it->is_array()) {
		int i = 0;
		for (auto& item : *it) {
			if (i < 18)
				SkillProfMod[i][0] = item[0].get<int>();
			i++;
		}
	}

	newFeat.init(name, desc, type, AbilityScoreMod, SaveProfMod, SkillProfMod, MaxHPMod, ACMod, SpeedMod);

	return newFeat;
}

/// <summary>
/// This function initializes a feat with its values.
/// All fields must be filled out, any unmodified value should have a -1
/// </summary>
/// <param name="Name -- std::string"></param>
/// <param name="Description -- std::string"></param>
/// <param name="Feature Type -- Class/Racial/Normal"></param>
/// <param name="AbilityScoreMod -- two-dimensional integer array, first param [] is the ability, second param [0] is the mod"></param>
/// <param name="SkillProfMod -- two-dimensional integer array, first param [] is the ability, second param [0] is None/Proficiency/Expertise"></param>
/// <param name="SaveProfMod -- two-dimensional integer array, first param [] is the skill, second param [0] is None/Proficiency"></param>
/// <param name="MaxHPMod -- integer representing MaxHP mod"></param>
/// <param name="ACMod -- integer representing AC mod"></param>
/// <param name="SpeedMod -- integer representing Speed mod"></param>
void Feature::init(std::string Name, std::string Desc, int newValue, int Abilityscoremod[6][1], int saveMod[6][1], int skillMod[18][1], int Maxhpmod, int ACmod, int Speedmod)
{
	// Value checks
	if (newValue < 0 || newValue > 2)
		newValue = NORMAL;

	name = Name;
	desc = Desc;
	FeatType = newValue;

	for (int i = 0; i < 6; i++)
		AbilityScoreMod[i][0] = Abilityscoremod[i][0];

	MaxHPMod = Maxhpmod;
	ACMod = ACmod;
	SpeedMod = Speedmod;

	for (int i = 0; i < 6; i++)
		SaveProfMod[i][0] = saveMod[i][0];

	for (int i = 0; i < 18; i++)
		SkillProfMod[i][0] = skillMod[i][0];
}

/// <summary>
/// This method is used if one feature unlocks another. The "parent" feature will subsequently apply the "child" features.
/// </summary>
void Feature::setChain(Feature *f)
{
	chain = f;
}

/// <summary>
/// This method undoes all changes made by the feature on the statblock.
/// </summary>
void Feature::remove(void* STAT)
{
	Stat* s = static_cast<Stat*>(STAT); // Cast the void pointer to a stat pointer.

	// Remove if the feat has been applied only
	if (applied)
	{
		applied = false;

		// Ability Score Mod
		for (int i = 0; i < 6; i++)
		{
			if (AbilityScoreMod[i][0] != -1)
				s->updateScore(i, s->getScoreMods()[i] - AbilityScoreMod[i][0]);
		}

		// Max HP Mod	
		if (MaxHPMod != -1)
			s->updateMaxHealth(s->getMaxHealth() - MaxHPMod);

		// Speed Mod
		if (SpeedMod != -1)
			s->updateSpeed(s->getSpeed() - SpeedMod);

		// Skill Prof Mod
		for (int i = 0; i < 18; i++)
		{
			if (SkillProfMod[i][0] != -1)
				s->updateSkillProf(i, SkillProfMod[i][0]);
		}

		// Save Prof Mod
		for (int i = 0; i < 6; i++)
		{
			if (SaveProfMod[i][0] != -1)
				s->updateSaveProf(i, SaveProfMod[i][0]);
		}

		// Apply any chained features to our statblock
		if (chain != nullptr)
			chain->remove(s);
	}
}

void Feature::update(void* STAT)
{
	Stat* s = static_cast<Stat*>(STAT); // Cast the void pointer to a stat pointer.

	// Apply changes for the first time
	if (!applied) 
	{
		applied = true;

		// Ability Score Mod
		for (int i = 0; i < 6; i++)
		{
			if (AbilityScoreMod[i][0] != -1)
				s->updateScore(i, s->getScoreMods()[i] + AbilityScoreMod[i][0]);
		}

		// Max HP Mod
		if (MaxHPMod != -1)
			s->updateMaxHealth(s->getMaxHealth() + MaxHPMod);

		// Speed Mod
		if (SpeedMod != -1)
			s->updateSpeed(s->getSpeed() + SpeedMod);
		
		// Skill Prof Mod
		for (int i = 0; i < 18; i++)
		{
			if (SkillProfMod[i][0] != -1)
				s->updateSkillProf(i, SkillProfMod[i][0]);
		}

		// Save Prof Mod
		for (int i = 0; i < 6; i++)
		{
			if (SaveProfMod[i][0] != -1)
				s->updateSaveProf(i, SaveProfMod[i][0]);
		}

		// Apply any chained features to our statblock
		if (chain != nullptr)
			chain->update(s);
	}
}

std::string Feature::printFeat()
{
	return name + ": " + desc;
}
