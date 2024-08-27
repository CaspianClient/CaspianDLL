#pragma once
#include <string>
#include <Vectors.hpp>
#include "../Client/Client.hpp"

class Utils {
public:
	static std::string getClientFolder() {
		return getenv("AppData") + (std::string)"\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\Caspian";
	};

	static std::string getRoamingStateFolder() {
		return getenv("AppData") + (std::string)"\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState";
	};

	static bool MouseInRect(Vec2 pos, Vec2 size) {
		if (Client::MousePos.x >= pos.x && Client::MousePos.x <= (pos.x + size.x) &&
			Client::MousePos.y >= pos.y && Client::MousePos.y <= (pos.y + size.y)) {
			return true;
		}

		return false;
	}

	static bool RectClippingOutside(Vec2& pos, Vec2 size) {
		bool clip = false;
		if (0.5 > pos.x) {
			pos.x = 0.5;
			clip = true;
		}
		else if (Client::WindowSize.x < pos.x + size.x) {
			pos.x = Client::WindowSize.x - size.x;
			clip = true;
		}
		if (0.5 > pos.y) {
			pos.y = 0.5;
			clip = true;
		}
		else if (pos.y + size.y > Client::WindowSize.y) {
			pos.y = Client::WindowSize.y - size.y;
			clip = true;
		}

		return clip;
	}

	static Vec2 CenterRect(Vec2 PaddingSize, Vec2 RectSize, Vec2 PaddingPos = Vec2()) {
		return PaddingPos + ((PaddingSize / 2) - (RectSize / 2));
	}

	static void onButtonClick(Vec2 ButtonPos, Vec2 ButtonSize, auto Func) {
		if (MouseInRect(ButtonPos, ButtonSize) and Client::MouseClickLeft) {
			return Func();
		}
	}

	static float lerp(float& current, float endValue, float delta) {
		current = current + (endValue - current) * delta;
		return current;
	}
};