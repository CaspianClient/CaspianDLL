#include "ModMenu.hpp"
#include "../../ModuleManager.hpp"
#include "../../../Render/RenderUtils/SettingsMenu/SettingsMenu.hpp"

ImColor col = ImColor(23, 245, 89, 255);

void ModMenu::RenderModMenu() {
	SizeComponent MenuSize(.9, .6);
	Vec2 MenuPos = Utils::CenterRect(PositionComponent(1, 1), MenuSize);
	//RndrUtils.RoundedShadows(MenuPos, MenuSize, ImColor(50, 50, 50, 255), 0, Client::WindowSize.y * 0.1);
	RndrUtils.RoundedRectFilled(MenuPos, MenuSize, ImColor(40, 40, 40));

	SizeComponent InnerRectSize(.88, .58);
	Vec2 InnerRectPos = Utils::CenterRect(PositionComponent(1, 1), InnerRectSize);
	RndrUtils.RoundedRectFilled(InnerRectPos, InnerRectSize, ImColor(80, 80, 80));

	SizeComponent ModuleRectSize(.86, .46);
	SizeComponent ModuleRectSizeDecoy(.86, .56);
	Vec2 ModuleRectPos = Utils::CenterRect(PositionComponent(1, 1), ModuleRectSizeDecoy);
	ModuleRectPos.y += Client::WindowSize.y * 0.1;
	RndrUtils.RoundedRectFilled(ModuleRectPos, ModuleRectSize, ImColor(40, 40, 40));

	SizeComponent ModCardSpacing(.01, .01);
	ModuleRectPos.x += ModCardSpacing.x;
	ModuleRectPos.y += ModCardSpacing.y;
	if (CurrModSetting != "") {
		RndrUtils.PushClipRect(ModuleRectPos - ModCardSpacing, ModuleRectSize);
		auto mod = ModuleMgr.GetModuleList()[CurrModSetting];
		mod->RenderSettings();
		mod->SettingPos = SettingScrollAmount;
		RndrUtils.PopClipRect();
	}
	else
		for (auto x : ModuleMgr.GetModuleList()) {
			if (x.first == this->getName()) continue;
			RenderModcard(x.second, ModuleRectPos);
			ModuleRectPos.y += Client::WindowSize.y * 0.09;
		}
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