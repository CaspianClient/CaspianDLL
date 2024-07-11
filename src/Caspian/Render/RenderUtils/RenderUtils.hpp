#pragma once
#include "../ImGUI/imgui.h"
#include "../../Resources/ResourceManager.hpp"
#include "../../Client/Client.hpp"
#include "Components/SizeComponent.hpp"
#include "Components/PositionComponent.hpp"
#include "Components/ColorComponent.hpp"
#include <string>
#include <Vectors.hpp>
#include <map>

enum DrawListType {
	ForeGround,
	BackGround
};

class RenderUtils {
private:
	//Drawlist Type
	DrawListType CurrentDrawList = BackGround;

	//Fonts
	std::map<std::string, ImFont*> FontList;
	bool FontSetup = false;
	

// ImGui Config Settings
public:
	void SetCurrentDrawList(DrawListType DrawList) {
		CurrentDrawList = DrawList;
	}

	ImDrawList* getDrawList() {
		switch (CurrentDrawList)
		{
		case ForeGround:
			return ImGui::GetForegroundDrawList();
		case BackGround:
			return ImGui::GetBackgroundDrawList();
		default:
			return nullptr;
		}
	}

	void SetupFonts() {
		Resource mcFont = GET_RESOURCE("MinecraftFont");
		FontList["MinecraftFont"] = ImGui::GetIO().Fonts->AddFontFromMemoryTTF((void*)mcFont.data, mcFont.size, 100);
		FontSetup = true;
	}

//Utilities
public:
	void PushClipRect(Vec2 Position, Vec2 Size) {
		getDrawList()->PushClipRect(Position, Position + Size);
	}

	void PopClipRect() {
		getDrawList()->PopClipRect();
	}

//Rendering Shapes
public:
	void RoundedRectFilled(Vec2 Position, Vec2 Size, ImColor Colour, float Rounding = 0.0f, int Flags = 240) {
		getDrawList()->AddRectFilled(Position, Position + Size, Colour, Rounding, Flags);
	}

	void RoundedRectBorder(Vec2 Position, Vec2 Size, ImColor Colour, float Thickness, float Rounding = 0.0f, int Flags = 240) {
		getDrawList()->AddRect(Position, Position + Size, Colour, Rounding, Flags, Thickness);
	}

	void RectMultiColor(Vec2 Position, Vec2 Size, ImColor topLeft, ImColor topRight, ImColor bottomLeft, ImColor bottomRight) {
		getDrawList()->AddRectFilledMultiColor(Position, Position + Size, topLeft, topRight, bottomRight, bottomLeft);
	}

//Rendering Other Things
public:
	void Text(Vec2 Position, Vec2 PaddingSize, ImColor TextColor, std::string Text, float FontSize, int Alignment, std::string Font = "MinecraftFont") {

		if (!FontSetup) {
			SetupFonts();
		}

		ImGui::PushFont(FontList[Font]);
		float fSize = FontSize * Client::WindowSize.y / 1080;

		ImGui::SetWindowFontScale(fSize);

		switch (Alignment) {
			case 1: 
				break;

			case 2: {
				Position.x += (PaddingSize.x / 2) - (ImGui::CalcTextSize(Text.c_str()).x / 2);
				break;
			}

			case 3: {
				Position.x += (PaddingSize.x - ImGui::CalcTextSize(Text.c_str()).x);
				break;
			}
		}

		Position.y += (PaddingSize.y / 2) - (ImGui::CalcTextSize(Text.c_str()).y / 2);
		getDrawList()->AddText(Position, TextColor, Text.c_str());
		ImGui::PopFont();
	}
};

extern inline RenderUtils RndrUtils = RenderUtils();