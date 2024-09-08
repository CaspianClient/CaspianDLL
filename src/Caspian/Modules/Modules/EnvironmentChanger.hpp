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
        EventDispatcher.listen<GetTimeOfDayEvent>(getTimeOfDayEvent);

		this->set("enabled", false, false);
        this->set("rain", false, false);
		this->set("rainLevel", 0, false);
        this->set("lightning", false, false);
		this->set("lightninglevel", 0, false);
        this->set("ChangeTime", false, false);
        this->set("time", 0, false);
	}

	void RenderSettings() override {
        AddToggle("rain", "Enable Rain");
        if (get<bool>("rain"))
		    AddSlider("rainLevel", "Rain Amount", 0, 5);
        AddToggle("lightning", "Enable Lightning");
        if (get<bool>("lightning"))
		    AddSlider("lightninglevel", "Lightning Amount", 0, 10);
        AddToggle("ChangeTime", "Change Time");
        if (get<bool>("ChangeTime"))
            AddSlider("time", "Time", 0, 1);
	}

	std::function<void(TickEvent &)> tickEvent = [&](TickEvent &event)
	{
		if (!this->get<bool>("enabled")) return;

        auto weather = SDK::CI->getBlockSource()->dimension->weather;

        weather->RainLevel = get<float>("rainLevel");
		weather->LightningLevelOld = get<float>("lightninglevel");
        weather->LightningLevelNew = get<float>("lightninglevel");
		
	};

    std::function<void(GetTimeOfDayEvent &)> getTimeOfDayEvent = [&](GetTimeOfDayEvent &event){
        if (!this->get<bool>("enabled") or !get<bool>("ChangeTime")) return;

        event.time = get<float>("time");
    };

};