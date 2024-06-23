#pragma once
#include "../Hook.hpp"
#include "../../../Events/Event.hpp"

class KeyboardFeed : public Hook {
public:
	typedef void(__thiscall *detour)(int key, bool state);
	static inline detour func_original = 0;

	KeyboardFeed() : Hook("Keyboard Feed", "Keyboard::feed") {
		this->HookFunc(callback, (void**)&func_original);
	}

	static void callback(int key, bool state) {
		nes::event_holder<KeyboardEvent> event;
		event->key = key;
		event->state = state;
		EventDispature.trigger(event);

		return func_original(key, state);
	}
};