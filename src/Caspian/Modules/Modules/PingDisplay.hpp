#include "../Module.hpp"

class PingDisplay : public Module {
public:
	PingDisplay() : Module("Ping Display", "Shows you ping.") {
		this->set("enabled", false, false);
		this->set("posX", 0, false);
		this->set("posY", 0, false);
		this->set("Size", 1, false);
		this->set("Rounding", 0, false);
		this->set("BGcolor", std::vector<float>{0, 0, 0, .5f}, false);
		this->set("TEXTcolor", std::vector<float>{1, 1, 1, 1}, false);

		EventDispatcher.listen<RenderEvent>(renderEvent);
        EventDispatcher.listen<GetAveragePingEvent>(getAveragePingEvent);
	}

	std::function<void(RenderEvent&)> renderEvent = [&](RenderEvent& event) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - LastPing).count();
        if (elapsedTime > 1.0f)
            ping = 0;
		this->RenderHud(std::to_string(ping) + " ms");
	};

    std::function<void(GetAveragePingEvent&)> getAveragePingEvent = [&](GetAveragePingEvent& event) {
        ping = event.ping;
        LastPing = std::chrono::high_resolution_clock::now();
    };

	void RenderSettings() override {
		AddSlider("Size", "Size", 0.5, 2.5);
		AddSlider("Rounding", "Rounding", 0, 1);
		AddColorPicker("BGcolor", "Background Color");
		AddColorPicker("TEXTcolor", "Text Color");
	}
private:
	int ping;
	std::chrono::time_point<std::chrono::high_resolution_clock> LastPing = std::chrono::high_resolution_clock::now();
};