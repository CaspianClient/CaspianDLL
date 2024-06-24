#pragma once
#include "../Module.hpp"
#include "../../Events/Event.hpp"
#include "../../Config/ConfigManager.hpp"
#include <functional>

class Zoom : public Module {
public:

	bool EnableZoom = false;
	float fov = 30;

	Zoom() : Module("Zoom", "Optifine Zoom") {
		EventDispature.listen<KeyboardEvent>(KeyEvent);
		EventDispature.listen<GetFOVevent>(getFovEvent);
		EventDispature.listen<MouseScrollEvent>(mScrollEvent);

		ConfigMgr.set(this->getName(), "enabled", true, false);
	}

	std::function<void(KeyboardEvent&)> KeyEvent = [&](KeyboardEvent& event) {
		if (event.key == 67) {
			EnableZoom = event.state;
			if (event.state == false) {
				fov = 30;
			}
		};
	};

	std::function<void(GetFOVevent&)> getFovEvent = [&](GetFOVevent& event) {
		if (ConfigMgr.get<bool>(this->getName(), "enabled") and EnableZoom) {
			event.fov = fov;
		}
	};

	std::function<void(MouseScrollEvent&)> mScrollEvent = [&](MouseScrollEvent& event) {
		if (ConfigMgr.get<bool>(this->getName(), "enabled") and EnableZoom) {
			event.mCancel = true;
			if (event.ScrollUp) {
				if (fov <= 15)
					fov = fov - 7;
				else
					fov = fov - 15;
			}
			else {
				fov = fov + 15;
			}

			if (fov > 90) fov = 90;
			else if (fov < 1) fov = 1;
		}
	};
};