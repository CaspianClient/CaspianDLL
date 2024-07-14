#pragma once
#include "../ImGUI/imgui.h"
#include "Components/SizeComponent.hpp"
#include "Components/PositionComponent.hpp"
#include "Components/ColorComponent.hpp"

#include <d3d11.h>
#include <d3d12.h>

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
	DrawListType CurrentDrawList = DrawListType::BackGround;

	//Fonts
	std::map<std::string, ImFont*> FontList;
	bool FontSetup = false;

	//Images
	std::map<std::string, ID3D11ShaderResourceView*> images = {};
	std::map<std::string, ImTextureID> imagesDX12 = {};

// helper functions
private:
	bool LoadTextureFromFileDX12(std::string resourceName, ID3D12Device* d3d_device, D3D12_CPU_DESCRIPTOR_HANDLE srv_cpu_handle, ID3D12Resource** out_tex_resource, int* out_width, int* out_height);

	bool LoadTextureFromFile(std::string resourceName, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
	
// ImGui Config Settings
public:
	void SetCurrentDrawList(DrawListType DrawList);

	ImDrawList* getDrawList();

	void SetupFonts();
//Utilities
public:
	void PushClipRect(Vec2 Position, Vec2 Size);

	void PopClipRect();

//Rendering Shapes
public:
	void RoundedRectFilled(Vec2 Position, Vec2 Size, ImColor Colour, float Rounding = 0.0f, int Flags = 240);

	void RoundedRectBorder(Vec2 Position, Vec2 Size, ImColor Colour, float Thickness, float Rounding = 0.0f, int Flags = 240);

	void RectMultiColor(Vec2 Position, Vec2 Size, ImColor topLeft, ImColor topRight, ImColor bottomLeft, ImColor bottomRight);

//Rendering Other Things
public:
	void Text(Vec2 Position, Vec2 PaddingSize, ImColor TextColor, std::string Text, float FontSize, int Alignment, std::string Font = "MinecraftFont");

	void RenderImage(Vec2 Pos, Vec2 size, std::string image, ImColor imgColour);

};

extern inline RenderUtils RndrUtils = RenderUtils();