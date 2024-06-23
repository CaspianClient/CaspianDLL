#include "ModuleManager.hpp"
#include "Modules/Zoom.hpp"

void ModuleManager::AddModule(Module* mod) {
	Modules[mod->getName()] = mod;
}

void ModuleManager::IntializeModules() {
	AddModule(new Zoom());
}

ModuleManager ModuleMgr = ModuleManager();