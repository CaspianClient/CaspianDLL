#include "ModMenu.hpp"
#include "../../../Render/RenderUtils/SettingsMenu/SettingsMenu.hpp"

void ModMenu::RenderModcard(Module* mod, Vec2 pos) {
	//SizeComponent ModuleRectSize(.86, .46);
	SizeComponent ModCardSize(.84, .08);
	RndrUtils.RoundedRectFilled(pos, ModCardSize, ImColor(100, 100, 100));

	SizeComponent OuterRimSize(.835, .075);
	Vec2 OuterRimPos = Utils::CenterRect(ModCardSize, OuterRimSize, pos);
	RndrUtils.RoundedRectBorder(OuterRimPos - Vec2(1, 1), OuterRimSize + Vec2(2, 2), ImColor(70, 70, 70), Client::WindowSize.y * 0.005);

	pos.x += Client::WindowSize.y * 0.02;
	RndrUtils.Text(pos, ModCardSize, IM_COL32_WHITE, mod->getName(), .45, 1);


	Vec2 TogglePos(pos.x + ModCardSize.x - (Client::WindowSize.y * 0.09), pos.y + (Client::WindowSize.y * 0.03));
	bool ModEnabled = mod->get<bool>("enabled");
	SettingsMenu::Toggle(TogglePos, ModEnabled);
	mod->set("enabled", ModEnabled);

	SizeComponent SettingSize(.04, .04);
	Vec2 SettingPos(pos.x + ModCardSize.x - (Client::WindowSize.y * 0.15), pos.y + (Client::WindowSize.y * 0.02));
	RndrUtils.RenderImage(SettingPos, SettingSize, "Settings", ImColor(60, 60, 60));

	Utils::onButtonClick(SettingPos, SettingSize, [&]() {
		SettingScrollAmount = 0;
		this->CurrModSetting = mod->getName();
		});
}