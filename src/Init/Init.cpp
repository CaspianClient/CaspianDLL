#include "Init.hpp"
#include "../Memory/Signatures/SigManager.hpp"
#include "../Memory/Hooks/HookManager.hpp"
#include "../Modules/ModuleManager.hpp"
#include "../Events/Event.hpp"
#include "../Config/ConfigManager.hpp"

bool Init::disabled = false;

void Init::Initialize() {
	SigManager::IntializeSigs();
	ConfigMgr.loadConfig();
	HookManager::InitializeHooks();
	ModuleMgr.IntializeModules();
	ConfigMgr.saveConfig();
	EventDispature.listen<KeyboardEvent>([&](KeyboardEvent& event) {
		if (event.key == 46) {
			EjectClient();
		}
		});
	return;
}

void Init::EjectClient() {
	MH_DisableHook(MH_ALL_HOOKS);
	printf("Client Ejected\n");
	disabled = true;
}

bool Init::isDisabled() {
	return disabled;
}