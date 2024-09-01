#pragma once
#include "../../src/Caspian/Render/ImGUI/imgui.h"

class Vec2 : public ImVec2 {
public:
	Vec2(float x = 0, float y = 0) {
		this->x = x;
		this->y = y;
	}

	Vec2 operator+(const Vec2 Vec) {
		return Vec2(this->x + Vec.x, this->y + Vec.y);
	}

	Vec2 operator-(const Vec2 Vec) {
		return Vec2(this->x - Vec.x, this->y - Vec.y);
	}

	Vec2 operator/(const float num) {
		return Vec2(this->x / num, this->y / num);
	}

	Vec2 operator*(const float num) {
		return Vec2(this->x * num, this->y * num);
	}

	Vec2 operator*(const Vec2 Vec) {
		return Vec2(this->x * Vec.x, this->y * Vec.y);
	}

	friend Vec2 operator*(float scalar, const Vec2& vec) {
        return Vec2(vec.x * scalar, vec.y * scalar);
    }
};

class Vec3 {
public:
	float x, y, z;

	Vec3(float x = 0.f, float y = 0.f, float z = 0.f) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};