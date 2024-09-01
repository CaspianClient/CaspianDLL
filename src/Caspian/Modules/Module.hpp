#pragma once
#include <string>
#include "../Render/RenderUtils/RenderUtils.hpp"
#include "../Config/ConfigManager.hpp"

class Module {
	std::string name;
	std::string description;

	std::map<std::string, bool> ColorPicker;
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

	void handleDragging(Vec2& pos, const Vec2& totalModuleSize) {
        if (Utils::MouseInRect(pos, totalModuleSize)) {
            if (Client::MouseHoldLeft && !isDragging) {
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

};