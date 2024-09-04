#pragma once
#include "../Module.hpp"
#include "../../Events/Event.hpp"
#include "../../Config/ConfigManager.hpp"
#include "../../SDK/SDK.hpp"
#include <functional>

class Fullbright : public Module
{
public:

	Fullbright() : Module("Fullbright", "Fullbright")
	{
		EventDispatcher.listen<GetGammaEvent>(getGammaEvent);

		this->set("enabled", false, false);
		this->set("amount", 25.0f, false);
	}

	void RenderSettings() override {
		AddSlider("amount", "Gamma Value", 0, 25);
	}

	std::function<void(GetGammaEvent &)> getGammaEvent = [&](GetGammaEvent &event)
	{
		if (this->get<bool>("enabled"))
			event.gamma = get<float>("amount");
		
	};

};