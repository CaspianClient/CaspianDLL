#pragma once
#include <map>
#include <string.h>
#include "Module.hpp"

class ModuleManager {
public:
	std::map<std::string, Module*> Modules = {};
	void IntializeModules();
	void AddModule(Module* mod);
};

extern ModuleManager ModuleMgr;