#pragma once
#include "../Module.hpp"
#include "../../Events/Event.hpp"
#include "../../Config/ConfigManager.hpp"
#include "../../SDK/SDK.hpp"
#include <functional>

class ModMenu : public Module {
	ModMenu() : Module("Mod Menu", "Mod Menu") {
		this->set("enabled", false, false);
		this->set("keybind", 77, false);
	}

	std::function<void(KeyboardEvent&)> KeyEvent = [&](KeyboardEvent& event) {

	};

	std::function<void(RenderEvent&)> renderEvent = [&](RenderEvent& event) {
		//this->RenderHud("FPS: " + std::to_string(Client::FPS));
	};
};
