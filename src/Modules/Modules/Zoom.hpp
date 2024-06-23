#pragma once
#include "../Module.hpp"
#include "../../Events/Event.hpp"
#include "../../Config/ConfigManager.hpp"
#include <functional>

class Zoom : public Module {
public:

	bool EnableZoom = false;

	Zoom() : Module("Zoom", "Optifine Zoom") {
		EventDispature.listen<KeyboardEvent>(KeyEvent);
		EventDispature.listen<GetFOVevent>(getFovEvent);

		ConfigMgr.set(this->getName(), "enabled", true, false);
	}

	std::function<void(KeyboardEvent&)> KeyEvent = [&](KeyboardEvent& event) {
		if (event.key == 67) {
			EnableZoom = event.state;
		};
	};

	std::function<void(GetFOVevent&)> getFovEvent = [&](GetFOVevent& event) {
		if (ConfigMgr.get<bool>(this->getName(), "enabled") and EnableZoom) {
			event.fov = 30.0f;
		}
	};
};