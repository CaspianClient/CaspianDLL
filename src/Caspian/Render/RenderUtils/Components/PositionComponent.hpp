#pragma once

#include <Vectors.hpp>

class PositionComponent : public Vec2 {
public:
	PositionComponent(float x, float y);
private:
    Vec2 position;
};
