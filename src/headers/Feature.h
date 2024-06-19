#ifndef FEATURE_H
#define FEATURE_H

#include <string>

class Feature {
public:
    Feature();
    void init(std::string name, std::string desc, int type, int Abilityscoremod[6][1], int saveMod[6][1], int skillMod[18][1], int Maxhpmod, int ACmod, int Speedmod);
	void setChain(Feature* f);
    void remove(void* STAT);
    void update(void* STAT);
    std::string printFeat();

    // Override == operator for comparison
    bool operator==(const Feature& other) const {
        return identifier == other.identifier;
    }

    int identifier;

private:
    std::string name;
    std::string desc;
    int FeatType;

    // Stat modifiables
    int AbilityScoreMod[6][1];
    int SaveProfMod[6][1];
	int SkillProfMod[18][1];
	int MaxHPMod, ACMod, SpeedMod;
    bool applied;

    Feature* chain;
};
#endif