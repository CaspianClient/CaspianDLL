#pragma once
#include "../Module.hpp"
#include "../../Events/Event.hpp"
#include "../../Config/ConfigManager.hpp"
#include "../../SDK/SDK.hpp"
#include <functional>

class SnapLook : public Module
{
public:
	bool EnableSnapLook = false;

	SnapLook() : Module("Snap Look", "Changes your perspective to Third Person Front.")
	{
		EventDispatcher.listen<KeyboardEvent>(KeyEvent);
		EventDispatcher.listen<GetPerspectiveEvent>(getPerspectiveEvent);

		this->set("enabled", false, false);
		this->set("keybind", 71, false);
	}

	void RenderSettings() override {
		AddKeybindPicker("keybind", "Keybind");
	}

	std::function<void(KeyboardEvent &)> KeyEvent = [&](KeyboardEvent &event)
	{
		if (event.key == get<int>("keybind"))
		{
			EnableSnapLook = event.state;
		};
	};

	std::function<void(GetPerspectiveEvent &)> getPerspectiveEvent = [&](GetPerspectiveEvent &event)
	{
		if (this->get<bool>("enabled") and EnableSnapLook and SDK::TopLayer == "hud_screen")
		{
			event.perspective = 2;
		}
	};
};