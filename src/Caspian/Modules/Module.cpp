#include "Module.hpp"
#include "../Render/RenderUtils/SettingsMenu/SettingsMenu.hpp"
#include "../SDK/SDK.hpp"
#include "ModuleManager.hpp"

void Module::RenderHud(const std::string text) {
	if (!this->get<bool>("enabled") or SDK::TopLayer != "hud_screen") {
		return;
	}
	PositionComponent pos = PositionComponent(this->get<float>("posX"), this->get<float>("posY"));
	float Size = this->get<float>("Size");
	SizeComponent size = SizeComponent(.15 * Size, .045 * Size);
	float MaximumSize = 2.5;
	float MinimumSize = .5;

	handleDragging(pos, size);

	float SmallerSide = min(size.x, size.y);

	float rounding = SmallerSide / 2 * get<float>("Rounding");

	RndrUtils.RoundedRectFilled(pos, size, RndrUtils.VecToImcolor(get<std::vector<float>>("BGcolor")), rounding);
	RndrUtils.Text(pos, size, RndrUtils.VecToImcolor(get<std::vector<float>>("TEXTcolor")), text, .35 * Size, 2);
}

void Module::handleDragging(Vec2& pos, const Vec2& totalModuleSize) {
    if (Utils::MouseInRect(pos, totalModuleSize)) {
        if (Client::MouseHoldLeft && !isDragging) {
            for(auto x : ModuleMgr.GetModuleList()){
                if (x.second->isDragging)
                    return;
            }
            isDragging = true;
            CursorPosHeld = Vec2(Client::MousePos.x - pos.x, Client::MousePos.y - pos.y);
        }
    }

    if (!Client::MouseHoldLeft)
        isDragging = false;

    if (isDragging) {
        pos.x = Client::MousePos.x - CursorPosHeld.x;
        pos.y = Client::MousePos.y - CursorPosHeld.y;

        Utils::RectClippingOutside(pos, totalModuleSize);

        set("posX", pos.x / Client::WindowSize.x);
        set("posY", pos.y / Client::WindowSize.y);
    }
}

void Module::AddToggle(std::string Setting, std::string DisplayName) {

	SizeComponent ModuleRectSizeDecoy(.86, .56);
	Vec2 ModuleRectPos = Utils::CenterRect(PositionComponent(1, 1), ModuleRectSizeDecoy);
	ModuleRectPos.y += Client::WindowSize.y * (0.12 + SettingPos);
	ModuleRectPos.x += Client::WindowSize.y * 0.02;
	
	bool value = this->get<bool>(Setting);
	SettingsMenu::Toggle(ModuleRectPos, value, 1.3);
	this->set(Setting, value);

	ModuleRectPos.x += Client::WindowSize.y * 0.07;
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
	Vec2 ModulePos = Utils::CenterRect(PositionComponent(1, 1), ModuleRectSizeDecoy);
	if (SettingPos >= .46) return;
	ModulePos.y += Client::WindowSize.y * 0.12;
	ModulePos.x += Client::WindowSize.y * 0.017;
	float ScrollModifier = Client::WindowSize.y * SettingPos;

	Vec2 pos = ModulePos;
	pos.y += ScrollModifier;

	std::vector<float> value = get<std::vector<float>>(Setting);

	ImColor col = RndrUtils.VecToImcolor(value);

	SizeComponent colorRect(.03, .03);

    RndrUtils.RenderImage(pos, colorRect, "TransparentRect", IM_COL32_WHITE);
    RndrUtils.RoundedRectFilled(pos, colorRect, col);

	Vec2 TextPos = pos;

	TextPos.x += Client::WindowSize.y * 0.038;

	RndrUtils.Text(TextPos, Vec2(0, colorRect.y), IM_COL32_WHITE, DisplayName, .35, 1);

	if (Utils::MouseInRect(pos, colorRect) and Client::MouseClickLeft) {
		if(!ColorPicker[Setting])
			ColorPicker[Setting] = true;
	}

	SizeComponent Pickerbackground(0.45, 0.35);

	if (!Utils::MouseInRect(pos, Pickerbackground) and Client::MouseClickLeft) {
		if(ColorPicker[Setting])
			ColorPicker[Setting] = false;
	}

	if (ColorPicker[Setting]) {
		RndrUtils.SetCurrentDrawList(ForeGround);
		SettingsMenu::ColorPicker(pos, col);
		RndrUtils.SetCurrentDrawList(BackGround);
		set(Setting, RndrUtils.ImcolorToVec(col));
		SettingPos += 0.35;
	}

	SettingPos += 0.047;
}

void Module::AddKeybindPicker(std::string Setting, std::string DisplayName) {
	SizeComponent ModuleRectSizeDecoy(.86, .56);
	Vec2 ModuleRectPos = Utils::CenterRect(PositionComponent(1, 1), ModuleRectSizeDecoy);
	ModuleRectPos.y += Client::WindowSize.y * (0.12 + SettingPos);
	ModuleRectPos.x += Client::WindowSize.y * 0.02;
	
	int value = this->get<int>(Setting);
	SettingsMenu::KeybindPicker(ModuleRectPos, value, Activekeybinds[Setting]);
	this->set(Setting, value);

	SizeComponent KeybindRectSize(0.11, 0.04);

	ModuleRectPos.x += KeybindRectSize.x;
	RndrUtils.Text(ModuleRectPos, Vec2(0, KeybindRectSize.y), IM_COL32_WHITE, DisplayName, .35, 1);
}