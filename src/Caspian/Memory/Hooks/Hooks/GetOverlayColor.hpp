#pragma once
#include "../Hook.hpp"
#include "../../../Events/Event.hpp"

class GetOverlayColor : public Hook
{
public:
	typedef mce::Color*(__thiscall *detour)(void *a1, mce::Color* color, __int64 a3);
	static inline detour func_original = 0;

	GetOverlayColor() : Hook("RenderController GetOverlayColor", "GetOverlayColor")
	{
		this->HookFunc(callback, (void **)&func_original);
	}

	static mce::Color* callback(void *a1, mce::Color* color, __int64 a3)
	{
		func_original(a1, color, a3);
		nes::event_holder<GetOverlayEvent> event;
		event->color = *color;
		EventDispatcher.trigger(event);
		*color = event->color;
		return color;
	}
};