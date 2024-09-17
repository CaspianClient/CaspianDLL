#pragma once
#include "../Hook.hpp"
#include "../../../Events/Event.hpp"

class RenderMobEffectsUI : public Hook
{
public:
	typedef float(__thiscall *detour)(__int64 a1, __int64 a2, __int64 a3, __int64 a4, int a5, void *a6);
	static inline detour func_original = 0;

	RenderMobEffectsUI() : Hook("RenderMobEffectsUI", "HudMobEffectsRenderer_render")
	{
		this->HookFunc(callback, (void **)&func_original);
	}

	static float callback(__int64 a1, __int64 a2, __int64 a3, __int64 a4, int a5, void *a6)
	{
		nes::event_holder<RenderMobEffectsUIEvent> event;
		EventDispatcher.trigger(event);
		if (!event->mCancel)
			return func_original(a1, a2, a3, a4, a5, a6);
	}
};