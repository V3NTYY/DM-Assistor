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
	repeatable = false;

	for (int i = 0; i < 6; i++)
		AcceptableVariants[i] = -1;

	for (int i = 0; i < 7; i++)
		AbilityScoreMod[i] = -1;

	for (int i = 0; i < 7; i++)
		SaveProfMod[i] = -1;

	for (int i = 0; i < 18; i++)
		SkillProfMod[i] = -1;

	MaxHPMod = -1;
	ACMod = -1;
	SpeedMod = -1;
	initiativeMod = -1;
	selectedVariant = -1;
}

void Feature::saveFeat(Feature f, std::string file)
{
	std::string chainName = "Null";
	if(f.chain != nullptr)
		chainName = f.chain->name;

	/// Temp variables
	int AcceptableVariants[6] = { -1, -1, -1, -1, -1, -1 };
	int AbilityScoreMod[7] = { -1, -1, -1, -1, -1, -1, -1 };
	int SaveProfMod[7] = { -1, -1, -1, -1, -1, -1, -1 };
	int SkillProfMod[18] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

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
		{"speedMod", f.SpeedMod},
		{"initiativeMod", f.initiativeMod},
		{"acceptableVariants", f.AcceptableVariants},
		{"repeatable", f.repeatable},
		{"chain", chainName}
	};

	/// Filter out and don't save values that are -1 or Null
	if (f.MaxHPMod == -1)
		j.erase("maxHPMod");
	if (f.ACMod == -1)
		j.erase("ACMod");
	if (f.SpeedMod == -1)
		j.erase("speedMod");
	if (std::equal(std::begin(AcceptableVariants), std::end(AcceptableVariants), std::begin(f.AcceptableVariants)))
		j.erase("acceptableVariants");
	if (std::equal(std::begin(AbilityScoreMod), std::end(AbilityScoreMod), std::begin(f.AbilityScoreMod)))
		j.erase("abilityScoreMod");
	if (std::equal(std::begin(SaveProfMod), std::end(SaveProfMod), std::begin(f.SaveProfMod)))
		j.erase("saveProfMod");
	if (std::equal(std::begin(SkillProfMod), std::end(SkillProfMod), std::begin(f.SkillProfMod)))
		j.erase("skillProfMod");
	if (f.initiativeMod == -1)
		j.erase("initiativeMod");
	if (chainName == "Null")
		j.erase("chain");

	if (!o) {
		wxMessageBox("Failed to open/create JSON.", "Error", wxICON_ERROR | wxOK);
		return;
	}

	o << j.dump();

	if (!o)
		wxMessageBox("Failed to write feature to JSON.", "Error", wxICON_ERROR | wxOK);
}

Feature Feature::loadFeat(std::string file, int Variant)
{
	Feature newFeat = Feature();

	// Check if we are chaining a feature, splice and fix our string
	std::string chainList = "";
	size_t pos = file.find("|");
	std::vector<std::string> chainNames;
	if (pos != std::string::npos) {
		chainList = file.substr(pos + 1);
		file = file.substr(0, pos);
	}

	// Split the chain list into a vector
	std::vector<std::string> chainResult;
	std::string::size_type start = 0;
	std::string::size_type end = chainList.find('|');
	while (end != std::string::npos) {
		chainResult.push_back(chainList.substr(start, end - start));
		start = end + 1;
		end = chainList.find('|', start);
	}
	chainResult.push_back(chainList.substr(start));

	std::ifstream f(file);
	// If we can't find the file, just return an ungenerated feat. addFeat() will handle the error.
	if (!f)
		return newFeat;

	json data = json::parse(f);

	// Load data from JSON. If a field isn't found, it is set to Null or ignored (-1).
	std::string name = data.value("name", "Null");
	std::string desc = data.value("desc", "Null");
	int type = data.value("type", -1);
	int MaxHPMod = data.value("maxHPMod", -1);
	int ACMod = data.value("ACMod", -1);
	int SpeedMod = data.value("speedMod", -1);
	int InitiativeMod = data.value("initiativeMod", -1);
	bool Repeatable = data.value("repeatable", false);

	int AcceptableVariants[6] = { -1, -1, -1, -1, -1, -1 };
	int AbilityScoreMod[7] = { -1, -1, -1, -1, -1, -1, -1 };
	int SaveProfMod[7] = { -1, -1, -1, -1, -1, -1, -1 };
	int SkillProfMod[18] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

	/// Handle the arrays from JSON
	auto it = data.find("acceptableVariants");
	if (it != data.end() && it->is_array()) {
		int i = 0;
		for (auto& item : *it) {
			if (i < 6)
				AcceptableVariants[i] = item.get<int>();
			i++;
		}
	}

	it = data.find("abilityScoreMod");
	if (it != data.end() && it->is_array()) {
		int i = 0;
		for (auto& item : *it) {
			if (i < 7)
				AbilityScoreMod[i] = item.get<int>();
			i++;
		}
	}

	it = data.find("saveProfMod");
	if (it != data.end() && it->is_array()) {
		int i = 0;
		for (auto& item : *it) {
			if (i < 7)
				SaveProfMod[i] = item.get<int>();
			i++;
		}
	}

	it = data.find("skillProfMod");
	if (it != data.end() && it->is_array()) {
		int i = 0;
		for (auto& item : *it) {
			if (i < 18)
				SkillProfMod[i] = item.get<int>();
			i++;
		}
	}

	std::string appendChainList = name + "|";
	// Check and ensure we aren't duplicating any chains/feats
	for (std::string currName : chainResult) {
		if (currName == name)
			return newFeat;
		appendChainList += currName + "|";
	}

	// Attach the chain if it exists
	Feature* chainFeat = nullptr;
	std::string chainName = data.value("chain", "Null");
	if (chainName != "Null" && chainName != name)
		chainFeat = new Feature(Feature::loadFeat(chainName + ".json|" + appendChainList, -1));
	
	// Assign modifiers to feat
	std::copy_n(AcceptableVariants, 6, newFeat.AcceptableVariants);
	std::copy_n(AbilityScoreMod, 7, newFeat.AbilityScoreMod);
	std::copy_n(SaveProfMod, 7, newFeat.SaveProfMod);
	std::copy_n(SkillProfMod, 18, newFeat.SkillProfMod);
	newFeat.MaxHPMod = MaxHPMod;
	newFeat.ACMod = ACMod;
	newFeat.SpeedMod = SpeedMod;
	newFeat.initiativeMod = InitiativeMod;
	newFeat.repeatable = Repeatable;

	newFeat.init(name, desc, type, Variant);
	newFeat.setChain(chainFeat);

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
void Feature::init(std::string Name, std::string Desc, int newValue, int Variant)
{
	// Value checks
	if (newValue < 0 || newValue > 2)
		newValue = NORMAL;

	name = Name;
	desc = Desc;
	FeatType = newValue;
	selectedVariant = Variant;
}

/// <summary>
/// This method is used if one feature unlocks another. The "parent" feature will subsequently apply the "child" features.
/// </summary>
void Feature::setChain(Feature *f)
{
	chain = f;
}

Feature* Feature::getChain()
{
	return chain;
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
			if (AbilityScoreMod[i] != -1)
				s->updateScore(i, s->getScoreMods()[i] - AbilityScoreMod[i]);
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
			if (SkillProfMod[i] != -1)
				s->updateSkillProf(i, SkillProfMod[i]);
		}

		// Save Prof Mod
		for (int i = 0; i < 6; i++)
		{
			if (SaveProfMod[i] != -1)
				s->updateSaveProf(i, SaveProfMod[i]);
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

		// If we have a variant, apply the variant to the score (used for selecting intelligence vs wisdom for example)
		if ((selectedVariant > -1 && selectedVariant < 7) && AcceptableVariants[selectedVariant] != -1)
		{
			if (AbilityScoreMod[6] != -1)
				AbilityScoreMod[selectedVariant] = AbilityScoreMod[6];
			if (SaveProfMod[6] != -1)
				SaveProfMod[selectedVariant] = SaveProfMod[6];
		}

		// Ability Score Mod
		for (int i = 0; i < 6; i++)
		{
			if (AbilityScoreMod[i] != -1)
				s->updateScore(i, s->getScoreMods()[i] + AbilityScoreMod[i]);
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
			if (SkillProfMod[i] != -1)
				s->updateSkillProf(i, SkillProfMod[i]);
		}

		// Save Prof Mod
		for (int i = 0; i < 6; i++)
		{
			if (SaveProfMod[i] != -1)
				s->updateSaveProf(i, SaveProfMod[i]);
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

std::string Feature::printChain()
{
	if (chain == nullptr)
		return "";
	else {
		std::string t1 = chain->name;
		std::string t2 = chain->desc;
		if (t1 == "Null" || t2 == "Null")
			return "Couldn't load feat.";
		else
			return chain->name + ": " + chain->desc;
	}
}
