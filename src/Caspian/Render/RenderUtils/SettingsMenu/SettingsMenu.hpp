#pragma once
#include <Vectors.hpp>

class SettingsMenu {
public:
	static void Toggle(Vec2 pos, bool& value, float size = 1);
	static void Slider(Vec2 pos, float& value, float min, float max, float size = 1);
	static void ColorPicker(Vec2 pos, ImColor& color);
	static void KeybindPicker(Vec2 pos, int& key, bool& active);
};