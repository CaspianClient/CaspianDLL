#pragma once
#include "../Hook.hpp"
#include "../../../Events/Event.hpp"

class MouseFeed : public Hook
{
public:
	typedef void(__thiscall *detour)(void *MouseDevice, char button, char action, short mouse_x, short mouse_y, short relativeMouseX, short relativeMouseY, bool isScrolling);
	static inline detour func_original = 0;

	MouseFeed() : Hook("Mouse Feed", "Mouse::feed")
	{
		this->HookFunc(callback, (void **)&func_original);
	}

	static void callback(void *MouseDevice, char button, char action, short mouse_x, short mouse_y, short relativeMouseX, short relativeMouseY, bool isScrolling)
	{
		bool mCancel = false;
		if (button == MouseButton::Scroll)
		{
			nes::event_holder<MouseScrollEvent> ScrollEvent;
			ScrollEvent->ScrollUp = (MouseAction)action == MouseAction::SCROLL_UP;
			EventDispatcher.trigger(ScrollEvent);
			if (ScrollEvent->mCancel)
				mCancel = true;
		}
		nes::event_holder<MouseEvent> mouseEvent;
		mouseEvent->action = (MouseAction)action;
		mouseEvent->button = (MouseButton)button;
		mouseEvent->mouseX = mouse_x;
		mouseEvent->mouseY = mouse_y;

		EventDispatcher.trigger(mouseEvent);
		if (mouseEvent->mCancel)
			mCancel = true;

		if (!mCancel)
			return func_original(MouseDevice, button, action, mouse_x, mouse_y, relativeMouseX, relativeMouseY, isScrolling);
	}
};