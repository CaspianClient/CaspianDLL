#include "ModuleManager.hpp"
#include "../Logger/Logger.hpp"

#include "Modules/Zoom.hpp"
#include "Modules/FPS.hpp"
#include "Modules/ModMenu/ModMenu.hpp"
#include "Modules/Keystrokes.hpp"

void ModuleManager::AddModule(Module* mod) {
	Modules[mod->getName()] = mod;
}

void ModuleManager::IntializeModules() {
	AddModule(new Zoom());
	AddModule(new FPS());
	AddModule(new ModMenu());
	AddModule(new Keystrokes());
}

std::map<std::string, Module*> ModuleManager::GetModuleList() {
	return Modules;
}

ModuleManager ModuleMgr = ModuleManager();