#include "../Module.hpp"
#include "../../SDK/SDK.hpp"

class ReachCounter : public Module {
public:
	ReachCounter() : Module("Reach Counter", "Shows you ping.") {
		this->set("enabled", false, false);
		this->set("posX", 0, false);
		this->set("posY", 0, false);
		this->set("Size", 1, false);
		this->set("Rounding", 0, false);
		this->set("BGcolor", std::vector<float>{0, 0, 0, .5f}, false);
		this->set("TEXTcolor", std::vector<float>{1, 1, 1, 1}, false);

		EventDispatcher.listen<RenderEvent>(renderEvent);
        EventDispatcher.listen<GameModeAttackEvent>(gameModeAttackEvent);
	}

	std::function<void(RenderEvent&)> renderEvent = [&](RenderEvent& event) {
		this->RenderHud("Reach: " + std::format("{:.2f}", Distance));
	};

    std::function<void(GameModeAttackEvent&)> gameModeAttackEvent = [&](GameModeAttackEvent& event) {
		auto ent = reinterpret_cast<Actor*>(event.entity);
		float istance = SDK::CI->getLocalPlayer()->level->hitResult->distance();
		Distance = (float)istance;
        LastHit = std::chrono::high_resolution_clock::now();
    };

	void RenderSettings() override {
		AddSlider("Size", "Size", 0.5, 2.5);
		AddSlider("Rounding", "Rounding", 0, 1);
		AddColorPicker("BGcolor", "Background Color");
		AddColorPicker("TEXTcolor", "Text Color");
	}
private:
	float Distance;
	std::chrono::time_point<std::chrono::high_resolution_clock> LastHit = std::chrono::high_resolution_clock::now();
};