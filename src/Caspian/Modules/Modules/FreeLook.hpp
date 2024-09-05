#pragma once
#include "../Module.hpp"
#include "../../Events/Event.hpp"
#include "../../Config/ConfigManager.hpp"
#include "../../SDK/SDK.hpp"
#include <functional>

class FreeLook : public Module
{
public:
	bool EnableFreeLook = false;
	Vec2 OriginalRot = Vec2();
	Vec2 OriginalHeadRot = Vec2();

	FreeLook() : Module("FreeLook", "FreeLook")
	{
		EventDispatcher.listen<KeyboardEvent>(KeyEvent);
		EventDispatcher.listen<SetupAndRenderEvent>(setupAndRenderEvent);

		this->set("enabled", false, false);
		this->set("keybind", 86, false);
	}

	void RenderSettings() override {
		AddKeybindPicker("keybind", "Keybind");
	}

	std::function<void(KeyboardEvent &)> KeyEvent = [&](KeyboardEvent &event)
	{
		if (SDK::TopLayer == "hud_screen")
			if (event.key == get<int>("keybind"))
			{
				EnableFreeLook = event.state;
				auto RotationComp = SDK::CI->getLocalPlayer()->getActorRotationComponent();
				auto HeadRotationComp = SDK::CI->getLocalPlayer()->getActorHeadRotationComponent();
				if (event.state == true)
				{
					OriginalRot = RotationComp->Rot;
					OriginalHeadRot = HeadRotationComp->Rot;
				}
				else {
					RotationComp->Rot = OriginalRot;
					HeadRotationComp->Rot = OriginalHeadRot;
				}
			};
	};

	std::function<void(SetupAndRenderEvent &)> setupAndRenderEvent = [&](SetupAndRenderEvent &event)
	{
		if (this->get<bool>("enabled") and EnableFreeLook and SDK::TopLayer == "hud_screen")
		{
			auto RotationComp = SDK::CI->getLocalPlayer()->getActorRotationComponent();
			auto HeadRotationComp = SDK::CI->getLocalPlayer()->getActorHeadRotationComponent();
			RotationComp->Rot = OriginalRot;
			HeadRotationComp->Rot = OriginalHeadRot;
		}
	};
};