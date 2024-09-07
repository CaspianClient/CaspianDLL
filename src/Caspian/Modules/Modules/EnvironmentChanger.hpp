#pragma once
#include "../Module.hpp"
#include "../../Events/Event.hpp"
#include "../../Config/ConfigManager.hpp"
#include "../../SDK/SDK.hpp"
#include <functional>

class EnvironmentChanger : public Module
{
public:

	EnvironmentChanger() : Module("Environment Changer", "EnvironmentChanger")
	{
		EventDispatcher.listen<TickEvent>(tickEvent);

		this->set("enabled", false, false);
		this->set("fogLevel", 0, false);
		this->set("rainLevel", 0, false);
		this->set("lightninglevel", 0, false);
	}

	void RenderSettings() override {
		AddSlider("fogLevel", "Fog Amount", 0, 10);
		AddSlider("rainLevel", "Rain Amount", 0, 10);
		AddSlider("lightninglevel", "Lightning Amount", 0, 10);
	}

	std::function<void(TickEvent &)> tickEvent = [&](TickEvent &event)
	{
		if (!this->get<bool>("enabled")) return;

        auto weather = SDK::CI->getBlockSource()->dimension->weather;

		std::cout << weather->FogLevel << std::endl;
		std::cout << weather->RainLevel << std::endl;

        weather->FogLevel = get<float>("fogLevel");
        weather->RainLevel = get<float>("rainLevel");
		weather->LightningLevel = get<float>("lightninglevel");
		
	};

};