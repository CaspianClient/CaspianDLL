#pragma once
#include "../Hook.hpp"
#include "../../../Events/Event.hpp"

class GetFOV : public Hook {
public:
	typedef float(__thiscall *detour)(void* a1, float f, void* a2, void* a3);
	static inline detour func_original = 0;

	GetFOV() : Hook("Get FOV", "GetFOV") {
		this->HookFunc(callback, (void**)&func_original);
	}

	static float callback(void* a1, float f, void* a2, void* a3) {
		nes::event_holder<GetFOVevent> event;
		event->fov = func_original(a1, f, a2, a3);
		EventDispature.trigger(event);
		return event->fov;
	}
};