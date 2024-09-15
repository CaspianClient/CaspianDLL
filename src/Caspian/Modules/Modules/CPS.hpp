#include "../Module.hpp"

class CPS : public Module {
public:
	CPS() : Module("CPS", "Shows your Clicks Per Second") {
		this->set("enabled", false, false);
		this->set("posX", 0, false);
		this->set("posY", 0, false);
		this->set("Size", 1, false);
		this->set("Rounding", 0, false);
		this->set("BGcolor", std::vector<float>{0, 0, 0, .5f}, false);
		this->set("TEXTcolor", std::vector<float>{1, 1, 1, 1}, false);
		this->set("rightCPS", false, false);

		EventDispatcher.listen<RenderEvent>(renderEvent);
	}

	std::function<void(RenderEvent&)> renderEvent = [&](RenderEvent& event) {
		std::string text = std::to_string(Client::GetLeftCPS());
		if (get<bool>("rightCPS")) text += " | " + std::to_string(Client::GetRightCPS());
		this->RenderHud("CPS: " + text);
	};

	void RenderSettings() override {
		AddSlider("Size", "Size", 0.5, 2.5);
		AddSlider("Rounding", "Rounding", 0, 1);
		AddColorPicker("BGcolor", "Background Color");
		AddColorPicker("TEXTcolor", "Text Color");
		AddToggle("rightCPS", "Show Right CPS");
	}
};