#pragma once
#include <string>

class Utils {
public:
	static std::string getClientFolder() {
		return getenv("AppData") + (std::string)"\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\Caspian";
	};

	static std::string getRoamingStateFolder() {
		return getenv("AppData") + (std::string)"\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState";
	};
};