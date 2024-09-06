#include "../Module.hpp"

class FPS : public Module {
public:
	FPS() : Module("FPS", "Shows your Frames Per Second") {
		this->set("enabled", false, false);
		this->set("posX", 0, false);
		this->set("posY", 0, false);
		this->set("Size", 1, false);
		this->set("BGcolor", std::vector<float>{0, 0, 0, .5f}, false);
		this->set("TEXTcolor", std::vector<float>{1, 1, 1, 1}, false);

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