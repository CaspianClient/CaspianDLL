#include "../Module.hpp"
#include <ctime>

class Clock : public Module {
public:
	Clock() : Module("Clock", "Shows you real time.") {
		this->set("enabled", false, false);
		this->set("posX", 0, false);
		this->set("posY", 0, false);
		this->set("Size", 1, false);
		this->set("BGcolor", std::vector<float>{0, 0, 0, .5f}, false);
		this->set("TEXTcolor", std::vector<float>{1, 1, 1, 1}, false);
		this->set("24h", false, false);

		EventDispatcher.listen<RenderEvent>(renderEvent);
	}

	std::function<void(RenderEvent&)> renderEvent = [&](RenderEvent& event) {
		time_t currentTime;
		struct tm* timeInfo;
		char timeBuffer[10];

		time(&currentTime);
		timeInfo = localtime(&currentTime);

		if (get<bool>("24h")) {
			strftime(timeBuffer, sizeof(timeBuffer), "%H:%M", timeInfo);
		}
		else {
			strftime(timeBuffer, sizeof(timeBuffer), "%I:%M %p", timeInfo);
		}
		std::string formattedTime(timeBuffer);
		this->RenderHud(formattedTime);
	};

	void RenderSettings() override {
		AddSlider("Size", "Size", 0.5, 2.5);
		AddToggle("24h", "24 Hour Format");
		AddColorPicker("BGcolor", "Background Color");
		AddColorPicker("TEXTcolor", "Text Color");
	}
};