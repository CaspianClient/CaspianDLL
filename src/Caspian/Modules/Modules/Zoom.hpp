#pragma once
#include "../Module.hpp"
#include "../../Events/Event.hpp"
#include "../../Config/ConfigManager.hpp"
#include "../../SDK/SDK.hpp"
#include <functional>

class Zoom : public Module
{
public:
	bool EnableZoom = false;
	float fov = 30;

	Zoom() : Module("Zoom", "Optifine Zoom")
	{
		EventDispatcher.listen<KeyboardEvent>(KeyEvent);
		EventDispatcher.listen<GetFOVevent>(getFovEvent);
		EventDispatcher.listen<MouseScrollEvent>(mScrollEvent);

		this->set("enabled", true, false);
		this->set("keybind", 67, false);
	}

	void RenderSettings() override {
		AddKeybindPicker("keybind", "Keybind");
	}

	std::function<void(KeyboardEvent &)> KeyEvent = [&](KeyboardEvent &event)
	{
		if (event.key == get<int>("keybind"))
		{
			EnableZoom = event.state;
			if (event.state == false)
			{
				fov = 30;
			}
		};
	};

	std::function<void(GetFOVevent &)> getFovEvent = [&](GetFOVevent &event)
	{
		if (this->get<bool>("enabled") and EnableZoom and SDK::TopLayer == "hud_screen")
		{
			event.fov = fov;
		}
	};

	std::function<void(MouseScrollEvent &)> mScrollEvent = [&](MouseScrollEvent &event)
	{
		if (this->get<bool>("enabled") and EnableZoom and SDK::TopLayer == "hud_screen")
		{
			event.mCancel = true;
			if (event.ScrollUp)
			{
				if (fov <= 15)
					fov = fov - 7;
				else
					fov = fov - 15;
			}
			else
			{
				fov = fov + 15;
			}

			if (fov > 90)
				fov = 90;
			else if (fov < 1)
				fov = 1;
		}
	};
};