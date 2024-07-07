#include "ModuleManager.hpp"
#include "Modules/Zoom.hpp"
#include "../Logger/Logger.hpp"

void ModuleManager::AddModule(Module* mod) {
	Modules[mod->getName()] = mod;
}

void ModuleManager::IntializeModules() {
	AddModule(new Zoom());
}

std::map<std::string, Module*> ModuleManager::GetModuleList() {
	return Modules;
}

ModuleManager ModuleMgr = ModuleManager();