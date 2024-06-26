#include "Init.hpp"
#include "../Memory/Signatures/SigManager.hpp"
#include "../Memory/Hooks/HookManager.hpp"
#include "../Modules/ModuleManager.hpp"
#include "../Events/Event.hpp"
#include "../Config/ConfigManager.hpp"
#include "../Logger/Logger.hpp"
#include "../Render/Setup/Setup.hpp"

bool Init::disabled = false;

void Init::Initialize() {
	Logger::SetupLogger();
	SigManager::IntializeSigs();
	ConfigMgr.loadConfig();
	HookManager::InitializeHooks();
	ModuleMgr.IntializeModules();
	ConfigMgr.saveConfig();
	SetupImGUI::IntializeSetup();

	Logger::info("Injection Completed");

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