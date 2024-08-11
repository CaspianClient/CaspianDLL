#include "ModuleManager.hpp"
#include "../Logger/Logger.hpp"

#include "Modules/Zoom.hpp"
#include "Modules/FPS.hpp"
#include "Modules/ModMenu/ModMenu.hpp"

void ModuleManager::AddModule(Module* mod) {
	Modules[mod->getName()] = mod;
}

void ModuleManager::IntializeModules() {
	AddModule(new Zoom());
	AddModule(new FPS());
	AddModule(new ModMenu());
}

std::map<std::string, Module*> ModuleManager::GetModuleList() {
	return Modules;
}

ModuleManager ModuleMgr = ModuleManager();