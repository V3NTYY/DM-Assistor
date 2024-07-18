#ifndef FEATURE_H
#define FEATURE_H

#include <string>
#include "headers/json.hpp"
#include <fstream>
using json = nlohmann::json;

class Feature {
public:
    Feature();
    void static saveFeat(Feature f, std::string file);
	Feature static loadFeat(std::string file, int variant);

    void init(std::string name, std::string desc, int type, int variant);
	void setChain(Feature* f);
    Feature* getChain();
    void remove(void* STAT);
    void update(void* STAT);
    std::string printFeat();
    std::string printChain();

    // Override == operator for comparison
    bool operator==(const Feature& other) const {
        bool returnVal = true;
		if (name != other.name)
			returnVal = false;

        return returnVal;
    }

    /// Only public because I'm too lazy to make getters. DONT TOUCH CHAIN
    std::string name;
    std::string desc;
    int FeatType;

    // Stat modifiables
    int AbilityScoreMod[7];
    int SaveProfMod[7];
    int SkillProfMod[18];
    int MaxHPMod, ACMod, SpeedMod;
    int initiativeMod;
    int AcceptableVariants[6];
    int selectedVariant;
    bool repeatable;
    bool applied;

private:
    Feature* chain;
};
#endif