#include "../SettingsMenu.hpp"
#include "../../RenderUtils.hpp"
#include "../../../../Utils/Utils.hpp"

void SettingsMenu::KeybindPicker(Vec2 pos, int& key, bool& active) {
	SizeComponent KeybindRectSize(0.1, 0.04);
	std::string KeyText = key != 0 ? Utils::GetKeyFromCode(key, true) : "N/A";

	ImColor bgColor = ImColor(128, 128, 128);
	ImColor textColor = active ? ImColor(225, 225, 84) : ImColor(0, 0, 0);

	RndrUtils.RoundedRectFilled(pos, KeybindRectSize, bgColor);
	RndrUtils.Text(pos, KeybindRectSize, textColor, KeyText, .35, 2);

	if (active) {
		if (Client::KeyThisFrame != 0) {
			key = Client::KeyThisFrame;
			active = false;
		}
		else if (!Utils::MouseInRect(pos, KeybindRectSize) and Client::MouseClickLeft) {
			key = 0;
			active = false;
		}
	}
	else if (Utils::MouseInRect(pos, KeybindRectSize) and Client::MouseClickLeft) {
		active = true;
	}
}