#include "ModuleManager.hpp"
#include "../Logger/Logger.hpp"

#include "Modules/Zoom.hpp"
#include "Modules/FPS.hpp"
#include "Modules/ModMenu/ModMenu.hpp"
#include "Modules/Keystrokes.hpp"
#include "Modules/Clock.hpp"
#include "Modules/ToggleSprint.hpp"
#include "Modules/CPS.hpp"
#include "Modules/Fullbright.hpp"
#include "Modules/FreeLook.hpp"
#include "Modules/SpeedDisplay.hpp"

void ModuleManager::AddModule(Module* mod) {
	Modules[mod->getName()] = mod;
}

void ModuleManager::IntializeModules() {
	AddModule(new Zoom());
	AddModule(new FPS());
	AddModule(new ModMenu());
	AddModule(new Keystrokes());
	AddModule(new Clock());
	AddModule(new ToggleSprint());
	AddModule(new CPS());
	AddModule(new Fullbright());
	AddModule(new FreeLook());
	AddModule(new SpeedDisplay());
}

std::map<std::string, Module*> ModuleManager::GetModuleList() {
	return Modules;
}

ModuleManager ModuleMgr = ModuleManager();