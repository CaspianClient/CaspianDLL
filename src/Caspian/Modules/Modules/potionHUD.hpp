#pragma once
#include "../Module.hpp"
#include "../../Events/Event.hpp"
#include "../../Config/ConfigManager.hpp"
#include "../../SDK/SDK.hpp"
#include <functional>
#include <unordered_map>
#include <string.h>

class potionHUD : public Module
{
public:

	potionHUD() : Module("Potion HUD", "Potion HUD")
	{
		EventDispatcher.listen<RenderEvent>(renderEvent);

		this->set("enabled", false, false);
		this->set("Size", 1, false);
        this->set("useEffectColor", true, false);
        this->set("TEXTcolor", std::vector<float>{1, 1, 1, 1}, false);

	}
	void RenderSettings() override {
		AddSlider("Size", "Size", 0.5, 2.5);
        AddToggle("useEffectColor", "Use Effect's Color");
        if (!get<bool>("useEffectColor"))
            AddColorPicker("TEXTcolor", "Text Color");
	}

	std::function<void(RenderEvent&)> renderEvent = [&](RenderEvent& event) {
		if (!get<bool>("enabled") or SDK::TopLayer != "hud_screen") return;

        float size = get<float>("Size");

        PositionComponent pos(0.01, 0.3);
        SizeComponent SizeEachEffect(0, 0.03 * size);
        float heightIncrement = 0;

        for (MobEffectInstance effect : SDK::CI->getLocalPlayer()->getMobEffectsComponent()->effects) {
            if ((int)effect.mId <= 0) continue;
            const int& totalSeconds = effect.mDuration / 20;
            const int& Minutes = totalSeconds / 60;
            const int& Seconds = totalSeconds % 60;

            Vec2 RenderPos = pos;
            RenderPos.y += heightIncrement;
            ImColor col = get<bool>("useEffectColor") ? EffectColors[(int)effect.mId] : RndrUtils.VecToImcolor(get<std::vector<float>>("TEXTcolor"));

            std::string EffectText = Effects[(int)effect.mId] + " " + Amplifiers[effect.mAmplifier + 1] + ": " + std::to_string(Minutes) + ":" + std::to_string(Seconds);

            RndrUtils.Text(RenderPos, SizeEachEffect, col, EffectText, 0.35 * size, 1);
            
            heightIncrement += SizeEachEffect.y;
        }
		
	};

private:
    std::unordered_map<int, std::string> Effects = {
    {static_cast<int>(MobEffectID::Speed), "Speed"},
    {static_cast<int>(MobEffectID::Slowness), "Slowness"},
    {static_cast<int>(MobEffectID::Haste), "Haste"},
    {static_cast<int>(MobEffectID::MiningFatigue), "Mining Fatigue"},
    {static_cast<int>(MobEffectID::Strength), "Strength"},
    {static_cast<int>(MobEffectID::InstantHealth), "Instant Health"},
    {static_cast<int>(MobEffectID::InstantDamage), "Instant Damage"},
    {static_cast<int>(MobEffectID::JumpBoost), "Jump Boost"},
    {static_cast<int>(MobEffectID::Nausea), "Nausea"},
    {static_cast<int>(MobEffectID::Regeneration), "Regeneration"},
    {static_cast<int>(MobEffectID::Resistance), "Resistance"},
    {static_cast<int>(MobEffectID::FireResistance), "Fire Resistance"},
    {static_cast<int>(MobEffectID::WaterBreathing), "Water Breathing"},
    {static_cast<int>(MobEffectID::Invisibility), "Invisibility"},
    {static_cast<int>(MobEffectID::Blindness), "Blindness"},
    {static_cast<int>(MobEffectID::NightVision), "Night Vision"},
    {static_cast<int>(MobEffectID::Hunger), "Hunger"},
    {static_cast<int>(MobEffectID::Weakness), "Weakness"},
    {static_cast<int>(MobEffectID::Poison), "Poison"},
    {static_cast<int>(MobEffectID::Wither), "Wither"},
    {static_cast<int>(MobEffectID::HealthBoost), "Health Boost"},
    {static_cast<int>(MobEffectID::Absorption), "Absorption"},
    {static_cast<int>(MobEffectID::Saturation), "Saturation"},
    {static_cast<int>(MobEffectID::Levitation), "Levitation"},
    {static_cast<int>(MobEffectID::FatalPoison), "Fatal Poison"},
    {static_cast<int>(MobEffectID::ConduitPower), "Conduit Power"},
    {static_cast<int>(MobEffectID::SlowFalling), "Slow Falling"},
    {static_cast<int>(MobEffectID::BadOmen), "Bad Omen"},
    {static_cast<int>(MobEffectID::VillageHero), "Village Hero"}
    };

    std::unordered_map<int, std::string> Amplifiers = {
        {1, "I"},
        {2, "II"},
        {3, "III"},
        {4, "IV"},
        {5, "V"},
        {6, "VI"},
        {7, "VII"}
    };

    std::unordered_map<int, ImColor> EffectColors = {
    {static_cast<int>(MobEffectID::Speed), ImColor(85, 255, 85)},           // Light Green
    {static_cast<int>(MobEffectID::Slowness), ImColor(96, 96, 255)},        // Light Blue
    {static_cast<int>(MobEffectID::Haste), ImColor(255, 255, 85)},          // Yellow
    {static_cast<int>(MobEffectID::MiningFatigue), ImColor(160, 160, 160)}, // Gray
    {static_cast<int>(MobEffectID::Strength), ImColor(255, 0, 0)},          // Red
    {static_cast<int>(MobEffectID::InstantHealth), ImColor(255, 85, 255)},  // Pink
    {static_cast<int>(MobEffectID::InstantDamage), ImColor(64, 0, 0)},      // Dark Red
    {static_cast<int>(MobEffectID::JumpBoost), ImColor(85, 255, 255)},      // Cyan
    {static_cast<int>(MobEffectID::Nausea), ImColor(85, 0, 0)},             // Dark Maroon
    {static_cast<int>(MobEffectID::Regeneration), ImColor(255, 85, 85)},    // Light Red
    {static_cast<int>(MobEffectID::Resistance), ImColor(192, 192, 192)},    // Silver
    {static_cast<int>(MobEffectID::FireResistance), ImColor(255, 170, 0)},  // Orange
    {static_cast<int>(MobEffectID::WaterBreathing), ImColor(0, 85, 255)},   // Blue
    {static_cast<int>(MobEffectID::Invisibility), ImColor(224, 224, 224)},  // Light Gray
    {static_cast<int>(MobEffectID::Blindness), ImColor(0, 0, 0)},           // Black
    {static_cast<int>(MobEffectID::NightVision), ImColor(85, 85, 255)},     // Blueish Purple
    {static_cast<int>(MobEffectID::Hunger), ImColor(128, 64, 0)},           // Brown
    {static_cast<int>(MobEffectID::Weakness), ImColor(96, 96, 96)},         // Dark Gray
    {static_cast<int>(MobEffectID::Poison), ImColor(85, 255, 85)},          // Green
    {static_cast<int>(MobEffectID::Wither), ImColor(48, 48, 48)},           // Dark Gray
    {static_cast<int>(MobEffectID::HealthBoost), ImColor(255, 170, 85)},    // Peach
    {static_cast<int>(MobEffectID::Absorption), ImColor(255, 170, 0)},      // Orange
    {static_cast<int>(MobEffectID::Saturation), ImColor(255, 85, 85)},      // Red
    {static_cast<int>(MobEffectID::Levitation), ImColor(170, 170, 255)},    // Light Purple
    {static_cast<int>(MobEffectID::FatalPoison), ImColor(0, 85, 0)},        // Dark Green
    {static_cast<int>(MobEffectID::ConduitPower), ImColor(0, 255, 255)},    // Aqua
    {static_cast<int>(MobEffectID::SlowFalling), ImColor(255, 255, 255)},   // White
    {static_cast<int>(MobEffectID::BadOmen), ImColor(0, 64, 64)},           // Dark Teal
    {static_cast<int>(MobEffectID::VillageHero), ImColor(85, 170, 85)}      // Green
    };
};