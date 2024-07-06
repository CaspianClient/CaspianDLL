#pragma once
#include "../ImGUI/imgui.h"
#include "../../Utils/Vectors.hpp"

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
};

extern RenderUtils RndrUtils = RenderUtils();