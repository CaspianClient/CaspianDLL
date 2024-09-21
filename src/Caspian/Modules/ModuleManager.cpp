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
#include "Modules/SnapLook.hpp"
#include "Modules/EnvironmentChanger.hpp"
#include "Modules/PingDisplay.hpp"
#include "Modules/Screenshot.hpp"
#include "Modules/MotionBlur.hpp"
#include "Modules/ReachCounter.hpp"
#include "Modules/potionHUD.hpp"
#include "Modules/MemoryDisplay.hpp"
#include "Modules/Tablist.hpp"

void ModuleManager::AddModule(Module* mod) {
	Modules[mod->getName()] = mod;
}

void ModuleManager::IntializeModules() {
	AddModule(new potionHUD());
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
	AddModule(new SnapLook());
    AddModule(new EnvironmentChanger());
    AddModule(new PingDisplay());
    AddModule(new Screenshot());
	AddModule(new ReachCounter());
	AddModule(new MemoryDisplay());
	AddModule(new Tablist());
	//AddModule(new MotionBlur());
}

std::map<std::string, Module*> ModuleManager::GetModuleList() {
	return Modules;
}

ModuleManager ModuleMgr = ModuleManager();