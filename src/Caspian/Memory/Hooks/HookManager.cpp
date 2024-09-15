#include "HookManager.hpp"
#include "Hooks/MouseFeed.hpp"
#include "Hooks/KeyboardFeed.hpp"
#include "Hooks/GetFOV.hpp"
#include "Hooks/SetupAndRender.hpp"
#include "Hooks/ActorBaseTick.hpp"
#include "Hooks/getGamma.hpp"
#include "Hooks/ActorIntersects.hpp"
#include "Hooks/GetPerspective.hpp"
#include "Hooks/DimensionGetDayTime.hpp"
#include "Hooks/GetAveragePing.hpp"
#include "Hooks/OnAppSuspend.hpp"

std::vector<Hook*> HookManager::Hooks = {};

void HookManager::InitializeHooks() {
	MH_Initialize();
	Hooks.push_back(new MouseFeed());
	Hooks.push_back(new KeyboardFeed());
	Hooks.push_back(new GetFOV());
	Hooks.push_back(new SetupAndRender());
	Hooks.push_back(new ActorBaseTick());
	Hooks.push_back(new GetGamma());
	Hooks.push_back(new ActorIntersects());
	Hooks.push_back(new GetPerspective());
    Hooks.push_back(new DimensionGetDayTime());
	Hooks.push_back(new GetAveragePing());
	Hooks.push_back(new OnAppSuspend());
}