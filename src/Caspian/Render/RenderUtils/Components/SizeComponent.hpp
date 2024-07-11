#pragma once

#include <Vectors.hpp>

class SizeComponent : public Vec2 {
public:
	SizeComponent(float x, float y);

	SizeComponent operator*(const int num) {
		return SizeComponent(this->size.x * num, this->size.y * num);
	}
private:
    Vec2 size;
};
