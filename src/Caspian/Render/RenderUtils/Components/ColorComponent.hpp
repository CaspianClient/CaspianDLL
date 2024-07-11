#pragma once

#include "../../ImGUI/imgui.h"
#include <string>

class ColorComponent {
public:
	ColorComponent(float r, float g, float b, float a = 255.f);
    explicit ColorComponent(const std::string& hex, float a = 255.f);

	[[nodiscard]] ImColor getImColor() const;
    [[nodiscard]] std::string getHex() const;

	void lerp(const ColorComponent& color, float t);

	static ImColor hexToImColor(const std::string& hexString, float alpha);
    static [[nodiscard]] std::string imColorToHex(const ImColor& color);

    float r;
    float g;
    float b;
    float a;
};
