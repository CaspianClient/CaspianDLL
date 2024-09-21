#pragma once
#include <string>
#include <Vectors.hpp>
#include <Windows.h>
#include <filesystem>
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

	static ImColor LerpImColor(ImColor& currentCol, ImColor endColor, float delta) {
		lerp(currentCol.Value.x, endColor.Value.x, delta);
		lerp(currentCol.Value.y, endColor.Value.y, delta);
		lerp(currentCol.Value.z, endColor.Value.z, delta);
		lerp(currentCol.Value.w, endColor.Value.w, delta);

		return currentCol;
	}

    static std::string GetKeyFromCode(int keycode, bool Keybind = false) {
        //Credits chat gpt
        std::unordered_map<int, std::string> shiftMap = {
            {0x30, ")"}, {0x31, "!"}, {0x32, "@"}, {0x33, "#"},
            {0x34, "$"}, {0x35, "%"}, {0x36, "^"}, {0x37, "&"},
            {0x38, "*"}, {0x39, "("}, {0xBA, ":"}, {0xBB, "+"},
            {0xBC, "<"}, {0xBD, "_"}, {0xBE, ">"}, {0xBF, "?"},
            {0xC0, "~"}, {0xDB, "{"}, {0xDC, "|"}, {0xDD, "}"},
            {0xDE, "\""}
        };

        std::unordered_map<int, std::string> keyMap = {
            {0x30, "0"}, {0x31, "1"}, {0x32, "2"}, {0x33, "3"},
            {0x34, "4"}, {0x35, "5"}, {0x36, "6"}, {0x37, "7"},
            {0x38, "8"}, {0x39, "9"}, {0x41, "a"}, {0x42, "b"},
            {0x43, "c"}, {0x44, "d"}, {0x45, "e"}, {0x46, "f"},
            {0x47, "g"}, {0x48, "h"}, {0x49, "i"}, {0x4A, "j"},
            {0x4B, "k"}, {0x4C, "l"}, {0x4D, "m"}, {0x4E, "n"},
            {0x4F, "o"}, {0x50, "p"}, {0x51, "q"}, {0x52, "r"},
            {0x53, "s"}, {0x54, "t"}, {0x55, "u"}, {0x56, "v"},
            {0x57, "w"}, {0x58, "x"}, {0x59, "y"}, {0x5A, "z"},
            {0xBA, ";"}, {0xBB, "="}, {0xBC, ","}, {0xBD, "-"},
            {0xBE, "."}, {0xBF, "/"}, {0xC0, "`"}, {0xDB, "["},
            {0xDC, "\\"}, {0xDD, "]"}, {0xDE, "'"}
        };

        bool capsLock = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
        bool shiftPressed = (GetKeyState(VK_SHIFT) & 0x8000) != 0;

        if (keycode >= 0x41 && keycode <= 0x5A) { // A-Z
            std::string key = keyMap[keycode];
            if ((capsLock && !shiftPressed) || (!capsLock && shiftPressed) || Keybind) {
                key[0] = toupper(key[0]);
            }
            return key;
        }

        if (keyMap.find(keycode) != keyMap.end()) {
            if (shiftPressed && shiftMap.find(keycode) != shiftMap.end() && !Keybind) {
                return shiftMap[keycode];
            }
            else {
                return keyMap[keycode];
            }
        }

        std::unordered_map<int, std::string> NonAlphaNumericKeyMap = {
            {0x08, "Backspace"},     {0x09, "Tab"},           {0x0D, "Enter"},
            {0x10, "Shift"},         {0x11, "Ctrl"},          {0x12, "Alt"},
            {0x14, "Caps"},          {0x1B, "Esc"},           {0x20, "Space"},
            {0x21, "Pg Up"},         {0x22, "Pg Down"},       {0x23, "End"},
            {0x24, "Home"},          {0x25, "Left"},          {0x26, "Up"},
            {0x27, "Right"},         {0x28, "Down"},          {0x2C, "Print Screen"},
            {0x2D, "Insert"},        {0x2E, "Delete"},        {0x5B, "Left Windows"},
            {0x5C, "Right Windows"}, {0x5D, "Applications"},  {0x60, "Num 0"},
            {0x61, "Num 1"},         {0x62, "Num 2"},         {0x63, "Num 3"},
            {0x64, "Num 4"},         {0x65, "Num 5"},         {0x66, "Num 6"},
            {0x67, "Num 7"},         {0x68, "Num 8"},         {0x69, "Num 9"},
            {0x6A, "Multiply"},      {0x6B, "Add"},           {0x6D, "Subtract"},
            {0x6E, "Decimal"},       {0x6F, "Divide"},        {0x70, "F1"},
            {0x71, "F2"},            {0x72, "F3"},            {0x73, "F4"},
            {0x74, "F5"},            {0x75, "F6"},            {0x76, "F7"},
            {0x77, "F8"},            {0x78, "F9"},            {0x79, "F10"},
            {0x7A, "F11"},           {0x7B, "F12"},           {0x90, "Num Lock"},
            {0x91, "Scroll Lock"},   {0xA0, "Left Shift"},    {0xA1, "Right Shift"},
            {0xA2, "Left Control"},  {0xA3, "Right Control"}, {0xA4, "Left Alt"},
            {0xA5, "Right Alt"}
        };

        return NonAlphaNumericKeyMap[keycode];
    }

    static void CreateFolder(std::string FolderPath) {
        if (!std::filesystem::exists(FolderPath)) {
            //std::filesystem::path p(FolderPath);
            std::filesystem::create_directories(FolderPath);
        }
    }

    static std::string StripColorCode(std::string input) {
        std::string result;
        result.reserve(input.size());  // Reserve space to avoid multiple allocations

        for (size_t i = 0; i < input.size(); ++i) {
            if (input[i] == '\xC2' && i + 1 < input.size() && input[i + 1] == '\xA7') { // § in UTF-8
                // Skip the '§' and the next character
                i += 2; // Move past the UTF-8 encoding and the character
            }
            else {
                result += input[i];
            }
        }

        return result;
    }

};