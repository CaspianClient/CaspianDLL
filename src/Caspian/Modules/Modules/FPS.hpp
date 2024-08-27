#include "../Module.hpp"

class FPS : public Module {
public:
	FPS() : Module("FPS", "Shows your Frames Per Second") {
		this->set("enabled", true, false);
		this->set("posX", 0, false);
		this->set("posY", 0, false);
		this->set("Size", 1, false);
		this->set("BGcolor", std::vector<float>{.1f, .1f, .1f, .6f});
		this->set("TEXTcolor", std::vector<float>{1, 1, 1, 1});

		EventDispatcher.listen<RenderEvent>(renderEvent);
	}

	std::function<void(RenderEvent&)> renderEvent = [&](RenderEvent& event) {
		this->RenderHud("FPS: " + std::to_string(Client::FPS));
	};

	void RenderSettings() override {
		AddSlider("Size", "Size", 0.5, 2.5);
		AddColorPicker("BGcolor", "Background Color");
		AddColorPicker("TEXTcolor", "Text Color");
	}
};