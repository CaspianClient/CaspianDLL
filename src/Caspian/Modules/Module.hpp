#pragma once
#include <string>
#include "../Render/RenderUtils/RenderUtils.hpp"
#include "../Config/ConfigManager.hpp"

class Module {
	std::string name;
	std::string description;

	std::map<std::string, bool> ColorPicker;
	std::map<std::string, bool> Activekeybinds;
public:
	float SettingPos = 0;
	bool isDragging = false;
	Vec2 CursorPosHeld = Vec2();
	bool enabled;
	Module(std::string mName, std::string mDescription) {
		this->name = mName;
		this->description = mDescription;
	}
	std::string getName() {
		return this->name;
	}

	bool CanRenderHud();

	bool AnyActiveKeybind() {
		for (auto key : Activekeybinds) {
			if (key.second) return true;
		}
		return false;
	}

	void handleDragging(Vec2& pos, const Vec2& totalModuleSize);

	template<typename T>
    T get(const std::string& key) const {
		return ConfigMgr.get<T>(this->name, key);
	}
    
    template<typename T>
    void set(const std::string& key, const T& value, bool override = true) {
		return ConfigMgr.set(this->name, key, value, override);
    }

	virtual void LoadSettings(bool Override = false) {}

	virtual void RenderSettings() {}

	virtual void RenderHud(const std::string text);

	void AddToggle(std::string Setting, std::string DisplayName);
	void AddSlider(std::string Setting, std::string DisplayName, float min, float max);
	void AddColorPicker(std::string Setting, std::string DisplayName);
	void AddKeybindPicker(std::string Setting, std::string DisplayName);
};