#pragma once
#include "../Hook.hpp"
#include "../../../Events/Event.hpp"
#include "../../../Config/ConfigManager.hpp"

class OnAppSuspend : public Hook
{
public:
	typedef void(__thiscall *detour)(void *a1);
	static inline detour func_original = 0;

	OnAppSuspend() : Hook("OnAppSuspend", "OnAppSuspend")
	{
		this->HookFunc(callback, (void **)&func_original);
	}

	static void callback(void *a1)
	{
		ConfigMgr.saveConfig();
		return func_original(a1);
	}
};