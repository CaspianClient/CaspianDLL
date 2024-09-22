#include "../Module.hpp"
#include "../../SDK/SDK.hpp"

class HurtColor : public Module {
public:
	HurtColor() : Module("Hurt Color", "HurtColor.") {
		this->set("enabled", false, false);
		this->set("color", std::vector<float>{1, 0, 0, .6f}, false);

		EventDispatcher.listen<GetOverlayEvent>(getOverlayEvent);
	}

	std::function<void(GetOverlayEvent&)> getOverlayEvent = [&](GetOverlayEvent& event) {
		if (get<bool>("enabled") and event.color.a > 0) {
			auto col = get<std::vector<float>>("color");
			event.color.r = col.at(0);
			event.color.g = col.at(1);
			event.color.b = col.at(2);
			event.color.a = col.at(3);
		}

	};

	void RenderSettings() override {
		AddColorPicker("color", "Hurt Color");
	}
private:
	bool Toggled = false;
};