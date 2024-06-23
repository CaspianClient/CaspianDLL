#pragma once
#include "../Module.hpp"
#include "../../Events/Event.hpp"
#include "../../Config/ConfigManager.hpp"
#include <functional>

class Zoom : public Module {
public:
	Zoom() : Module("Zoom", "Optifine Zoom") {
		EventDispature.listen<KeyboardEvent>(KeyEvent);

		ConfigMgr.set("Zoom", "enabled", true);
	}

	std::function<void(KeyboardEvent&)> KeyEvent = [&](KeyboardEvent& event) {
		if (event.key == 67) {
			printf("zoom zoom \n");
		};
	};
};