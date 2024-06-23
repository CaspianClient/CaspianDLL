#include "Init.hpp"
#include "../Memory/Signatures/SigManager.hpp"
#include "../Memory/Hooks/HookManager.hpp"
#include "../Events/Event.hpp"
#include <format>

bool Init::disabled = false;

void Init::Initialize() {
	SigManager::IntializeSigs();
	HookManager::InitializeHooks();

	EventDispature.listen<KeyboardEvent>([](KeyboardEvent& event) {
		printf(std::format("{} key\n", event.key).c_str());
		});

	return;
}

bool Init::isDisabled() {
	return disabled;
}