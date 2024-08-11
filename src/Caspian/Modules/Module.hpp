#pragma once
#include <string>
#include "../Render/RenderUtils/RenderUtils.hpp"
#include "../Config/ConfigManager.hpp"

class Module {
	std::string name;
	std::string description;
public:
	bool enabled;
	Module(std::string mName, std::string mDescription) {
		this->name = mName;
		this->description = mDescription;
	}
	std::string getName() {
		return this->name;
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

	virtual void RenderHud(const std::string text) {
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

		RndrUtils.RoundedRectFilled(pos, size, ImColor(.1f, .1f, .1f, .6f), 0);
		RndrUtils.Text(pos, size, IM_COL32_WHITE, text, .35 * Size, 2);
	}

private:
	bool isDragging = false;
	Vec2 CursorPosHeld = Vec2();
};