#include "../Module.hpp"

class FPS : public Module {
public:
	FPS() : Module("FPS", "Shows your Frames Per Second") {
		this->set("enabled", true, false);
		this->set("posX", 0, false);
		this->set("posY", 0, false);
		this->set("Size", 1);

		EventDispatcher.listen<RenderEvent>(renderEvent);
	}

	std::function<void(RenderEvent&)> renderEvent = [&](RenderEvent& event) {
		this->RenderHud("FPS: " + std::to_string(Client::FPS));
	};
};