#pragma once
#include "../Hook.hpp"
#include "../../../Events/Event.hpp"

class GetGamma : public Hook
{
public:
	typedef float(__thiscall *detour)(void *a1);
	static inline detour func_original = 0;

	GetGamma() : Hook("Get Gamma", "GetGamma")
	{
		this->HookFunc(callback, (void **)&func_original);
	}

	static float callback(void *a1)
	{
		nes::event_holder<GetGammaEvent> event;
		event->gamma = func_original(a1);
		EventDispatcher.trigger(event);
		return event->gamma;
	}
};