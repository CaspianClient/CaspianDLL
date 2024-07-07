#pragma once
#include "../ImGUI/imgui.h"
#include <string>
#include <Vectors.hpp>

enum DrawListType {
	ForeGround,
	BackGround
};

class RenderUtils {
private:
	DrawListType CurrentDrawList = BackGround;


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
	void Text(Vec2 Position, Vec2 PaddingSize, ImColor TextColor, std::string Text, float FontSize, int Alignment) {
		float fSize = FontSize;

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
	}
};

extern RenderUtils RndrUtils = RenderUtils();