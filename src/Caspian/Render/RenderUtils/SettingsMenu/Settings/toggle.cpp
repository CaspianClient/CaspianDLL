#include "../SettingsMenu.hpp"
#include "../../RenderUtils.hpp"
#include "../../../../Utils/Utils.hpp"

void SettingsMenu::Toggle(Vec2 pos, bool& value, float size) {
	SizeComponent ToggleBackground(0.04 * size, 0.02 * size);
	SizeComponent ToggleButton(0.02 * size, 0.03 * size);

	SizeComponent OuterRimToggleBackground(0.037 * size, 0.017 * size);
	SizeComponent OuterRimToggleButton(0.017 * size, 0.027 * size);

	SizeComponent ToggleOffSpacing(-0.01 * size, -0.005 * size);
	SizeComponent ToggleOnSpacing(0.03 * size, -0.005 * size);

	Vec2 PosOrig = pos;

	if (!value) {
		RndrUtils.RoundedRectFilled(pos, ToggleBackground, ImColor(161, 38, 38));

		Vec2 OuterRimBGpos = Utils::CenterRect(ToggleBackground, OuterRimToggleBackground, pos);
		RndrUtils.RoundedRectBorder(OuterRimBGpos - Vec2(1, 1), OuterRimToggleBackground + Vec2(2, 2), IM_COL32_BLACK, Client::WindowSize.y * 0.003 * size);

		pos.x += ToggleOffSpacing.x;
		pos.y += ToggleOffSpacing.y;

		RndrUtils.RoundedRectFilled(pos, ToggleButton, ImColor(60, 60, 60));

		Vec2 OuterRimTogglePos = Utils::CenterRect(ToggleButton, OuterRimToggleButton, pos);
		RndrUtils.RoundedRectBorder(OuterRimTogglePos - Vec2(1, 1), OuterRimToggleButton + Vec2(2, 2), IM_COL32_BLACK, Client::WindowSize.y * 0.003 * size);
	}
	else {
		RndrUtils.RoundedRectFilled(pos, ToggleBackground, ImColor(50, 186, 43));

		Vec2 OuterRimBGpos = Utils::CenterRect(ToggleBackground, OuterRimToggleBackground, pos);
		RndrUtils.RoundedRectBorder(OuterRimBGpos - Vec2(1, 1), OuterRimToggleBackground + Vec2(2, 2), IM_COL32_BLACK, Client::WindowSize.y * 0.003 * size);

		pos.x += ToggleOnSpacing.x;
		pos.y += ToggleOnSpacing.y;

		RndrUtils.RoundedRectFilled(pos, ToggleButton, ImColor(60, 60, 60));

		Vec2 OuterRimTogglePos = Utils::CenterRect(ToggleButton, OuterRimToggleButton, pos);
		RndrUtils.RoundedRectBorder(OuterRimTogglePos - Vec2(1, 1), OuterRimToggleButton + Vec2(2, 2), IM_COL32_BLACK, Client::WindowSize.y * 0.003 * size);
	}

	if (Utils::MouseInRect(PosOrig, ToggleBackground) and Client::MouseClickLeft) {
		value = !value;
	}
}