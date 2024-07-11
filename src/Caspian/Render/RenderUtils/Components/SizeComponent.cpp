#include "SizeComponent.hpp"

#include "../../../Client/Client.hpp"

SizeComponent::SizeComponent(const float x, const float y) {
	this->x = Client::WindowSize.y * x;
	this->y = Client::WindowSize.y * y;

	size = Vec2(x, y);
}
