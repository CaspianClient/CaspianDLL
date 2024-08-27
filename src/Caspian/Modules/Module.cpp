#include "Module.hpp"
#include "../Render/RenderUtils/SettingsMenu/SettingsMenu.hpp"

void Module::RenderHud(const std::string text) {
	if (!this->get<bool>("enabled")) {
		return;
	}
	PositionComponent pos = PositionComponent(this->get<float>("posX"), this->get<float>("posY"));
	float Size = this->get<float>("Size");
	SizeComponent size = SizeComponent(.15 * Size, .045 * Size);
	float MaximumSize = 2.5;
	float MinimumSize = .5;

	if (Utils::MouseInRect(pos, size)) {
		if (Client::ScrollUP) {
			float SetSize = this->get<float>("Size") + 0.2;

			if (SetSize <= MaximumSize) {
				this->set("Size", SetSize);
			}
		}

		else if (Client::ScrollDOWN) {
			float SetSize = this->get<float>("Size") - 0.2;

			if (SetSize >= MinimumSize) {
				this->set("Size", SetSize);
			}
		}
		if (Client::MouseHoldLeft and !isDragging) {
			isDragging = true;
			CursorPosHeld = Vec2(Client::MousePos.x - pos.x, Client::MousePos.y - pos.y);
		}
	}

	if (Client::MouseHoldLeft == false)
		isDragging = false;

	if (isDragging) {
		pos.x = Client::MousePos.x - CursorPosHeld.x;
		pos.y = Client::MousePos.y - CursorPosHeld.y;

		Utils::RectClippingOutside(pos, size);

		this->set("posX", pos.x / Client::WindowSize.x);
		this->set("posY", pos.y / Client::WindowSize.y);
	}

	RndrUtils.RoundedRectFilled(pos, size, RndrUtils.VecToImcolor(get<std::vector<float>>("BGcolor")), 0);
	RndrUtils.Text(pos, size, RndrUtils.VecToImcolor(get<std::vector<float>>("TEXTcolor")), text, .35 * Size, 2);
}

void Module::AddToggle(std::string Setting, std::string DisplayName) {

	SizeComponent ModuleRectSizeDecoy(.86, .56);
	Vec2 ModuleRectPos = Utils::CenterRect(PositionComponent(1, 1), ModuleRectSizeDecoy);
	ModuleRectPos.y += Client::WindowSize.y * (0.12 + SettingPos);
	ModuleRectPos.x += Client::WindowSize.y * 0.02;
	
	bool value = this->get<bool>(Setting);
	SettingsMenu::Toggle(ModuleRectPos, value, 1.3);
	this->set(Setting, value);

	ModuleRectPos.x += Client::WindowSize.y * 0.065;
	float ButtonYsize = Client::WindowSize.y * 0.02;
	RndrUtils.Text(ModuleRectPos, Vec2(0, ButtonYsize), IM_COL32_WHITE, DisplayName, .35, 1);

	SettingPos += 0.04;
}

void Module::AddSlider(std::string Setting, std::string DisplayName, float min, float max) {
	SizeComponent ModuleRectSizeDecoy(.86, .56);
	Vec2 ModuleRectPos = Utils::CenterRect(PositionComponent(1, 1), ModuleRectSizeDecoy);
	ModuleRectPos.y += Client::WindowSize.y * (0.12 + SettingPos);
	ModuleRectPos.x += Client::WindowSize.y * 0.02;

	float value = get<float>(Setting);

	float ButtonYsize = Client::WindowSize.y * 0.05;
	RndrUtils.Text(ModuleRectPos, Vec2(0, ButtonYsize), IM_COL32_WHITE, DisplayName + ": " + std::format("{:.2f}", value), .35, 1);
	ModuleRectPos.y += ButtonYsize;

	SettingsMenu::Slider(ModuleRectPos, value, min, max, 1.3);

	set(Setting, value);

	SettingPos += 0.1;
}

void Module::AddColorPicker(std::string Setting, std::string DisplayName) {
	SizeComponent ModuleRectSizeDecoy(.86, .56);
	Vec2 ModuleRectPos = Utils::CenterRect(PositionComponent(1, 1), ModuleRectSizeDecoy);
	if (SettingPos >= .46) return;
	ModuleRectPos.y += Client::WindowSize.y * (0.12 + SettingPos);
	ModuleRectPos.x += Client::WindowSize.y * 0.017;

	std::vector<float> value = get<std::vector<float>>(Setting);

	ImColor col = RndrUtils.VecToImcolor(value);

	SizeComponent colorRect(.03, .03);
	SizeComponent AlphabarIMG(0.27, 0.18);

	RndrUtils.SetCurrentDrawList(BackGround);
	RndrUtils.PushClipRect(ModuleRectPos, colorRect);
    RndrUtils.RenderImage(ModuleRectPos, AlphabarIMG, "Transparent", IM_COL32_WHITE);
    RndrUtils.PopClipRect();
    RndrUtils.RoundedRectFilled(ModuleRectPos, colorRect, col);

	Vec2 TextPos = ModuleRectPos;

	TextPos.x += Client::WindowSize.y * 0.038;

	RndrUtils.Text(TextPos, Vec2(0, colorRect.y), IM_COL32_WHITE, DisplayName, .35, 1);

	if (Utils::MouseInRect(ModuleRectPos, colorRect) and Client::MouseClickLeft) {
		if(!ColorPicker[Setting])
			ColorPicker[Setting] = true;
	}

	SizeComponent Pickerbackground(0.45, 0.35);

	if (!Utils::MouseInRect(ModuleRectPos, Pickerbackground) and Client::MouseClickLeft) {
		if(ColorPicker[Setting])
			ColorPicker[Setting] = false;
	}

	if (ColorPicker[Setting]) {
		RndrUtils.SetCurrentDrawList(ForeGround);
		SettingsMenu::ColorPicker(ModuleRectPos, col);
		RndrUtils.SetCurrentDrawList(BackGround);
		set(Setting, RndrUtils.ImcolorToVec(col));
		SettingPos += 0.35;
	}

	SettingPos += 0.047;
}