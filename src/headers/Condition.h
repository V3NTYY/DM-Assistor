#ifndef CONDITION_H
#define CONDITION_H
#include <string>

class Condition
{
public:
	Condition() : name("Null"), description("Null"), isApplied(false) {}
	Condition(std::string Name, std::string Desc, bool Active) : name(Name), description(Desc), isApplied(Active) {}
	std::string getName() const { return name; }
	std::string getDesc() const { return description; }
	bool isActive() const { return isApplied; }

	void setName(std::string newName) { name = newName; }
	void setDesc(std::string newDesc) { description = newDesc; }
	void setActive(bool newActive) { isApplied = newActive; }

private:
	std::string name;
	std::string description;
	bool isApplied;
};

#endif

