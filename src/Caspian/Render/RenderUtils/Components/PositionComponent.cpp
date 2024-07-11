#include "PositionComponent.hpp"

#include "../../../Client/Client.hpp"

PositionComponent::PositionComponent(const float x, const float y) {
	this->x = Client::WindowSize.x * x;
	this->y = Client::WindowSize.y * y;
}