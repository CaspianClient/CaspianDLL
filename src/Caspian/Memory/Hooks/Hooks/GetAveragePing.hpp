#pragma once
#include "../Hook.hpp"
#include "../../../Events/Event.hpp"

class GetAveragePing : public Hook
{
public:
	typedef __int64(__thiscall *detour)(void *a1, __int64 a2);
	static inline detour func_original = 0;

	GetAveragePing() : Hook("GetAveragePing", "GetAveragePing")
	{
		this->HookFunc(callback, (void **)&func_original);
	}

	static __int64 callback(void *a1, __int64 a2)
	{
		nes::event_holder<GetAveragePingEvent> event;
        event->ping = func_original(a1, a2);
        EventDispatcher.trigger(event);
        return event->ping;
	}
};