#include "HookManager.hpp"
#include "Hooks/MouseFeed.hpp"
#include "Hooks/KeyboardFeed.hpp"

std::vector<Hook*> HookManager::Hooks = {};

void HookManager::InitializeHooks() {
	MH_Initialize();
	Hooks.push_back(new MouseFeed());
	Hooks.push_back(new KeyboardFeed());
}