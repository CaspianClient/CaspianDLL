#pragma once
#include "../../Module.hpp"
#include "../../../Events/Event.hpp"
#include "../../../Config/ConfigManager.hpp"
#include "../../../SDK/SDK.hpp"
#include <functional>

class ModMenu : public Module {
public:
	ModMenu() : Module("Mod Menu", "Mod Menu") {
		this->set("enabled", false, false);
		this->set("keybind", 77, false);

		EventDispatcher.listen<RenderEvent>(renderEvent);
		EventDispatcher.listen<KeyboardEvent>(KeyEvent);
		EventDispatcher.listen<MouseEvent>(mouseEvent);
	}

	void RenderModcard(Module* mod, Vec2 pos);
	void RenderModMenu();

	std::string CurrModSetting = "";

	std::function<void(KeyboardEvent&)> KeyEvent = [&](KeyboardEvent& event) {
		if (this->get<bool>("enabled")) {
			event.mCancel = true;
		}

		if (event.key == this->get<int>("keybind") and event.state == false) {
			this->set("enabled", !this->get<bool>("enabled"));
			if (SDK::CI and SDK::TopLayer == "hud_screen") {
				if (this->get<bool>("enabled")) {
					SDK::CI->ReleaseMouse();
				}
				else {
					SDK::CI->GrabMouse();
				}
			}
		}

	};

	std::function<void(MouseEvent&)> mouseEvent = [&](MouseEvent& event) {
		if (this->get<bool>("enabled")) {
			event.mCancel = true;
		}
	};

	std::function<void(RenderEvent&)> renderEvent = [&](RenderEvent& event) {
		if (this->get<bool>("enabled"))
			RenderModMenu();
	};
};
