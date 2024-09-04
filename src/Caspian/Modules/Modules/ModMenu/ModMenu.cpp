#include "ModMenu.hpp"
#include "../../ModuleManager.hpp"
#include "../../../Render/RenderUtils/SettingsMenu/SettingsMenu.hpp"

ImColor col = ImColor(23, 245, 89, 255);

void ModMenu::RenderModMenu() {
	SizeComponent MenuSize(.9, .6);
	Vec2 MenuPos = Utils::CenterRect(PositionComponent(1, 1), MenuSize);
	RndrUtils.RoundedRectFilled(MenuPos, MenuSize, ImColor(40, 40, 40));

	SizeComponent InnerRectSize(.88, .58);
	Vec2 InnerRectPos = Utils::CenterRect(PositionComponent(1, 1), InnerRectSize);
	RndrUtils.RoundedRectFilled(InnerRectPos, InnerRectSize, ImColor(80, 80, 80));

	SizeComponent ModuleRectSize(.86, .46);
	SizeComponent ModuleRectSizeDecoy(.86, .56);
	Vec2 ModuleRectPos = Utils::CenterRect(PositionComponent(1, 1), ModuleRectSizeDecoy);
	ModuleRectPos.y += Client::WindowSize.y * 0.1;
	RndrUtils.RoundedRectFilled(ModuleRectPos, ModuleRectSize, ImColor(40, 40, 40));

	SizeComponent ModButtonSize(0.18, 0.06);
	SizeComponent SettingButtonSize(0.18, 0.06);
	float Spacing = SizeComponent(0.03, 0).x;

	Vec2 TotalButtonSizing = Vec2(ModButtonSize.x + Spacing + SettingButtonSize.x, SettingButtonSize.y);

	Vec2 ButtonsCentered = Utils::CenterRect(Vec2(ModuleRectSize.x, ModuleRectPos.y - InnerRectPos.y), TotalButtonSizing, InnerRectPos);

	ButtonsCentered.x = ModuleRectPos.x;

	Vec2 ModButtonPos = ButtonsCentered;

	Vec2 SettingButtonPos = (ButtonsCentered + TotalButtonSizing) - SettingButtonSize;

	RndrUtils.RoundedRectFilled(ModButtonPos, ModButtonSize, CurrModSetting == "" ? ImColor(120, 120, 120) : ImColor(100, 100, 100));
	RndrUtils.RoundedRectBorder(ModButtonPos, ModButtonSize, CurrModSetting == "" ? ImColor(60, 60, 60) : ImColor(140, 140, 140), Spacing/8);
	RndrUtils.Text(ModButtonPos, ModButtonSize, IM_COL32_WHITE, "Modules", .45, 2);
	Utils::onButtonClick(ModButtonPos, ModButtonSize, [&]() {
		CurrModSetting = "";
		SettingScrollAmount = 0;
		});

	RndrUtils.RoundedRectFilled(SettingButtonPos, SettingButtonSize, CurrModSetting != "" ? ImColor(120, 120, 120) : ImColor(100, 100, 100));
	RndrUtils.RoundedRectBorder(SettingButtonPos, SettingButtonSize, CurrModSetting != "" ? ImColor(60, 60, 60) : ImColor(140, 140, 140), Spacing/8);
	RndrUtils.Text(SettingButtonPos, SettingButtonSize, IM_COL32_WHITE, "Settings", .45, 2);
	Utils::onButtonClick(SettingButtonPos, SettingButtonSize, [&]() {
		if (CurrModSetting == "")
			SettingScrollAmount = 0;
			CurrModSetting = this->getName();
		});

	SizeComponent ModCardSpacing(.01, .01 + SettingScrollAmount);
	ModuleRectPos.x += ModCardSpacing.x;
	ModuleRectPos.y += ModCardSpacing.y;
	RndrUtils.PushClipRect(ModuleRectPos - ModCardSpacing, ModuleRectSize);
	if (CurrModSetting != "") {
		auto mod = ModuleMgr.GetModuleList()[CurrModSetting];
		mod->RenderSettings();
		mod->SettingPos = SettingScrollAmount;
	}
	else {
		for (auto x : ModuleMgr.GetModuleList()) {
			if (x.first == this->getName()) continue;
			RenderModcard(x.second, ModuleRectPos);
			ModuleRectPos.y += Client::WindowSize.y * 0.09;
		}
	}
	RndrUtils.PopClipRect();


}

void ModMenu::CurrSettingScroll(float amount) {
	ModuleMgr.GetModuleList()[CurrModSetting]->SettingPos += amount;
}

bool ModMenu::canClose() {
	for (auto m : ModuleMgr.GetModuleList()) {
		if (m.second->AnyActiveKeybind()) return false;
	}

	return true;
}