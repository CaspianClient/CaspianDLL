#include "ColorComponent.hpp"

ColorComponent::ColorComponent(const float r, const float g, const float b, const float a) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

ColorComponent::ColorComponent(const std::string& hex, const float a) {
    const ImColor color = hexToImColor(hex, a);

    this->r = color.Value.x * 255.f;
    this->g = color.Value.y * 255.f;
    this->b = color.Value.z * 255.f;
    this->a = color.Value.w * 255.f;
}

ImColor ColorComponent::getImColor() const {
	return ImColor(this->r, this->g, this->b, this->a);
}

std::string ColorComponent::getHex() const {
    return imColorToHex(ImColor(this->r / 255.f, this->g / 255.f, this->b / 255.f, this->a / 255.f));
}

void ColorComponent::lerp(const ColorComponent& color, const float t) {
	//this->r = Utils::lerp(color.r, this->r, t);
	//this->g = Utils::lerp(color.g, this->g, t);
	//this->b = Utils::lerp(color.b, this->b, t);
	//this->a = Utils::lerp(color.a, this->a, t);
}

ImColor ColorComponent::hexToImColor(const std::string& hexString, const float alpha) {
	if (hexString.length() < 6)
        return {0.f, 0.f, 0.f, alpha / 255.f};

	try {
		const uint32_t hex = std::stoul(hexString, nullptr, 16);
		const uint8_t red = hex >> 16 & 0xFF;
		const uint8_t green = hex >> 8 & 0xFF;
		const uint8_t blue = hex & 0xFF;

		const float normalizedRed = static_cast<float>(red) / 255.f;
		const float normalizedGreen = static_cast<float>(green) / 255.f;
		const float normalizedBlue = static_cast<float>(blue) / 255.f;

		return ImColor(normalizedRed, normalizedGreen, normalizedBlue, alpha / 255.f);
	}
	catch (const std::exception&) {
		return ImColor(0.f, 0.f, 0.f, alpha / 255.f);
	}
}

std::string ColorComponent::imColorToHex(const ImColor& color) {
    const auto red = static_cast<uint8_t>(color.Value.x * 255.0f);
    const auto green = static_cast<uint8_t>(color.Value.y * 255.0f);
    const auto blue = static_cast<uint8_t>(color.Value.z * 255.0f);

    char hexString[7];
    sprintf_s(hexString, 7, "%02x%02x%02x", red, green, blue);

    return hexString;
}
