#include "../Module.hpp"
#include "../../SDK/SDK.hpp"

class SpeedDisplay : public Module {
public:
	SpeedDisplay() : Module("Speed Display", "Shows your Blocks Per Second AKA Speed") {
		this->set("enabled", false, false);
		this->set("posX", 0, false);
		this->set("posY", 0, false);
		this->set("Size", 1, false);
		this->set("BGcolor", std::vector<float>{0, 0, 0, .5f});
		this->set("TEXTcolor", std::vector<float>{1, 1, 1, 1});

		EventDispatcher.listen<RenderEvent>(renderEvent);
		EventDispatcher.listen<TickEvent>(tickEvent);
	}

	std::function<void(RenderEvent&)> renderEvent = [&](RenderEvent& event) {
		this->RenderHud(std::format("{:.2f}", speed) + " M/S");
	};

	std::function<void(TickEvent&)> tickEvent = [&](TickEvent& event) {
		auto stateVector = SDK::CI->getLocalPlayer()->getStateVectorComponent();
		speed = LastPos.GetDistance(stateVector->pos) * 20;
		LastPos = stateVector->pos;
	};

	void RenderSettings() override {
		AddSlider("Size", "Size", 0.5, 2.5);
		AddColorPicker("BGcolor", "Background Color");
		AddColorPicker("TEXTcolor", "Text Color");
	}

private:
	Vec3 LastPos;
	float speed;
};