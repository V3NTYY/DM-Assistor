#ifndef SPELL_H
#define SPELL_H
#include <string>

class Spell
{
public:
	Spell();
	Spell(std::string name, std::string description, int level) : name(name), description(description), level(level) {}
	int getLevel() const { return level; }
	std::string getName() const { return name; }
	std::string getDescription() const { return description; }

private:
	std::string name;
	std::string description;
	int level;
};

#endif
