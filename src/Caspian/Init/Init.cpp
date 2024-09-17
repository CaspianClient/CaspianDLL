#include "Init.hpp"
#include "../Memory/Signatures/SigManager.hpp"
#include "../Memory/Hooks/HookManager.hpp"
#include "../Modules/ModuleManager.hpp"
#include "../Events/Event.hpp"
#include "../Config/ConfigManager.hpp"
#include "../Logger/Logger.hpp"
#include "../Render/Setup/Setup.hpp"
#include "../Resources/ResourceManager.hpp"
#include "../Client/Client.hpp"
#include "../Utils/Utils.hpp"
#include "../SDK/SDK.hpp"

bool Init::disabled = false;

void Init::Initialize()
{
	Utils::CreateFolder(Utils::getClientFolder() + "\\Screenshots");
	Utils::CreateFolder(Utils::getClientFolder() + "\\Logs");
	Utils::CreateFolder(Utils::getClientFolder() + "\\Configs");

	Logger::SetupLogger();
	SigManager::IntializeSigs();
	ConfigMgr.loadConfig();
	HookManager::InitializeHooks();
	Client::InitClientInfo();
	ModuleMgr.IntializeModules();
	ConfigMgr.saveConfig();
	resourceManager.LoadResources();
	SetupImGUI::IntializeSetup();

	Logger::info("Injection Completed");

	EventDispatcher.listen<KeyboardEvent>([&](KeyboardEvent &event)
																				{
		if (event.key == 46) {
			EjectClient();
		}
		});
	return;
}

void Init::EjectClient()
{
	MH_DisableHook(MH_ALL_HOOKS);
	printf("Client Ejected\n");
	disabled = true;
}

bool Init::isDisabled()
{
	return disabled;
}