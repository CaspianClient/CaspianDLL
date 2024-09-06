#pragma once
#include "../Hook.hpp"
#include "../../../Events/Event.hpp"

class GetPerspective : public Hook
{
public:
	typedef __int64(__thiscall *detour)(void *a1);
	static inline detour func_original = 0;

	GetPerspective() : Hook("Options GetPerspective", "GetPerspective")
	{
		this->HookFunc(callback, (void **)&func_original);
	}

	static __int64 callback(void *a1)
	{
		nes::event_holder<GetPerspectiveEvent> event;
		event->perspective = func_original(a1);
		EventDispatcher.trigger(event);

		return event->perspective;
	}
};