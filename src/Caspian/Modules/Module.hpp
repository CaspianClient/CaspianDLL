#pragma once
#include <string>

class Module {
	std::string name;
	std::string description;
public:
	bool enabled;
	Module(std::string mName, std::string mDescription) {
		this->name = mName;
		this->description = mDescription;
	}
	std::string getName() {
		return this->name;
	}

	virtual void LoadSettings(bool Override = false) {}

};