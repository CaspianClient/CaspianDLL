#pragma once
#include "../Hook.hpp"

class MouseFeed : public Hook {
public:
	typedef void(__thiscall *detour)(void* MouseDevice, char button, char action, short mouse_x, short mouse_y, short relativeMouseX, short relativeMouseY, bool isScrolling);
	static inline detour func_original = 0;

	MouseFeed() : Hook("Mouse Feed", "Mouse::feed") {
		this->HookFunc(callback, (void**)&func_original);
	}

	static void callback(void* MouseDevice, char button, char action, short mouse_x, short mouse_y, short relativeMouseX, short relativeMouseY, bool isScrolling) {
		return func_original(MouseDevice, button, action, mouse_x, mouse_y, relativeMouseX, relativeMouseY, isScrolling);
	}
};