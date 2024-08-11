#include "../SettingsMenu.hpp"
#include "../../RenderUtils.hpp"
#include "../../../../Utils/Utils.hpp"

void SettingsMenu::Toggle(Vec2 pos, bool& value, float size) {
	SizeComponent ToggleBackground(0.04, 0.02);
	SizeComponent ToggleButton(0.02, 0.03);

	SizeComponent OuterRimToggleBackground(0.037, 0.017);
	SizeComponent OuterRimToggleButton(0.017, 0.027);

	SizeComponent ToggleOffSpacing(-0.01, -0.005);
	SizeComponent ToggleOnSpacing(0.03, -0.005);

	Vec2 PosOrig = pos;

	if (!value) {
		RndrUtils.RoundedRectFilled(pos, ToggleBackground, ImColor(181, 38, 38));

		Vec2 OuterRimBGpos = Utils::CenterRect(ToggleBackground, OuterRimToggleBackground, pos);
		RndrUtils.RoundedRectBorder(OuterRimBGpos - Vec2(1, 1), OuterRimToggleBackground + Vec2(2, 2), ImColor(40, 40, 40), Client::WindowSize.y * 0.003);

		pos.x += ToggleOffSpacing.x;
		pos.y += ToggleOffSpacing.y;

		RndrUtils.RoundedRectFilled(pos, ToggleButton, ImColor(60, 60, 60));

		Vec2 OuterRimTogglePos = Utils::CenterRect(ToggleButton, OuterRimToggleButton, pos);
		RndrUtils.RoundedRectBorder(OuterRimTogglePos - Vec2(1, 1), OuterRimToggleButton + Vec2(2, 2), ImColor(40, 40, 40), Client::WindowSize.y * 0.003);
	}
	else {
		RndrUtils.RoundedRectFilled(pos, ToggleBackground, ImColor(50, 186, 43));

		Vec2 OuterRimBGpos = Utils::CenterRect(ToggleBackground, OuterRimToggleBackground, pos);
		RndrUtils.RoundedRectBorder(OuterRimBGpos - Vec2(1, 1), OuterRimToggleBackground + Vec2(2, 2), ImColor(40, 40, 40), Client::WindowSize.y * 0.003);

		pos.x += ToggleOnSpacing.x;
		pos.y += ToggleOnSpacing.y;

		RndrUtils.RoundedRectFilled(pos, ToggleButton, ImColor(60, 60, 60));

		Vec2 OuterRimTogglePos = Utils::CenterRect(ToggleButton, OuterRimToggleButton, pos);
		RndrUtils.RoundedRectBorder(OuterRimTogglePos - Vec2(1, 1), OuterRimToggleButton + Vec2(2, 2), ImColor(40, 40, 40), Client::WindowSize.y * 0.003);
	}

	if (Utils::MouseInRect(PosOrig, ToggleBackground) and Client::MouseClickLeft) {
		value = !value;
	}
}