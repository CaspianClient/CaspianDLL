#pragma once
#include "../Signatures/SigManager.hpp"
#include "Hook.hpp"
#include <MinHook.h>

class HookManager {
	static std::vector<Hook*> Hooks;
public:
	static void InitializeHooks();
};