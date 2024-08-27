#include "../SettingsMenu.hpp"
#include "../../RenderUtils.hpp"
#include "../../../../Utils/Utils.hpp"

void SettingsMenu::Slider(Vec2 pos, float& value, float min, float max, float size) {
	SizeComponent SliderBarSize(0.05 * size, 0.02 * size);
	SizeComponent SliderKnobSize(0.02 * size, 0.03 * size);
	
	SizeComponent OuterRimSliderBarSize(0.397 * size, 0.017 * size);
	SizeComponent OuterRimKnobSize(0.017 * size, 0.027 * size);

	if (Utils::MouseInRect(pos, OuterRimSliderBarSize) and Client::MouseHoldLeft) {
		float RelativeMousePos = Client::MousePos.x - pos.x;
		value = ((RelativeMousePos / OuterRimSliderBarSize.x) * (max - min)) + min;
	}

	Vec2 KnobPos = Vec2(((value - min) / (max - min)) * OuterRimSliderBarSize.x + pos.x - Client::WindowSize.y * (0.01 * size), pos.y - (OuterRimKnobSize.y/2 - OuterRimSliderBarSize.y/2));

	RndrUtils.RoundedRectBorder(pos, OuterRimSliderBarSize, ImColor(0, 0, 0), Client::WindowSize.y * 0.0076 * size);
	RndrUtils.RoundedRectFilled(pos, OuterRimSliderBarSize, ImColor(128, 128, 128), 0);

	RndrUtils.RoundedRectBorder(KnobPos, OuterRimKnobSize, ImColor(0, 0, 0), Client::WindowSize.y * 0.0076 * size);
	RndrUtils.RoundedRectFilled(KnobPos, OuterRimKnobSize, ImColor(198, 198, 198), 0);
}