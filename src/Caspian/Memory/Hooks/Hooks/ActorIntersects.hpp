#pragma once
#include "../Hook.hpp"
#include "../../../Events/Event.hpp"

class ActorIntersects : public Hook
{
public:
	typedef bool(__thiscall *detour)(void *a1, float * a2, float *a3);
	static inline detour func_original = 0;

	ActorIntersects() : Hook("Actor Intersects", "ActorIntersects")
	{
		this->HookFunc(callback, (void **)&func_original);
	}

	static bool callback(void *a1, float * a2, float *a3)
	{
		nes::event_holder<ActorIntersectsEvent> event;
		EventDispatcher.trigger(event);
		return func_original(a1, a2, a3);
	}
};