#include "HookManager.hpp"
#include "Hooks/MouseFeed.hpp"
#include "Hooks/KeyboardFeed.hpp"
#include "Hooks/GetFOV.hpp"
#include "Hooks/SetupAndRender.hpp"

std::vector<Hook*> HookManager::Hooks = {};

void HookManager::InitializeHooks() {
	MH_Initialize();
	Hooks.push_back(new MouseFeed());
	Hooks.push_back(new KeyboardFeed());
	Hooks.push_back(new GetFOV());
	Hooks.push_back(new SetupAndRender());
}