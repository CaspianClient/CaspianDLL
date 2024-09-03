#include "../Module.hpp"
#include "../../SDK/SDK.hpp"

class ToggleSprint : public Module {
public:
	ToggleSprint() : Module("Toggle Sprint", "Automatically Sprints for you.") {
		this->set("enabled", false, false);
		this->set("keybind", 17, false);
		this->set("always", false, false);

		EventDispatcher.listen<TickEvent>(tickEvent);
		EventDispatcher.listen<KeyboardEvent>(keyEvent);
	}

	std::function<void(TickEvent&)> tickEvent = [&](TickEvent& event) {
		if (!get<bool>("enabled")) return;
		auto moveInput = SDK::CI->getLocalPlayer()->getMoveInputHandler();
		if (get<bool>("always") or Toggled) moveInput->sprinting = true;

	};

	std::function<void(KeyboardEvent&)> keyEvent = [&](KeyboardEvent& event) {
		if (!get<bool>("enabled")) return;

		if (event.key == get<int>("keybind") && event.state == 1) { 
			Toggled = !Toggled;
			if (!Toggled) {
				auto CI = SDK::CI;
				if (CI and CI->getLocalPlayer())
					CI->getLocalPlayer()->getMoveInputHandler()->sprinting = false;
			}
		}
	};

	void RenderSettings() override {
		AddToggle("always", "Always Sprint");
		AddKeybindPicker("keybind", "Keybind");
	}
private:
	bool Toggled = false;
};