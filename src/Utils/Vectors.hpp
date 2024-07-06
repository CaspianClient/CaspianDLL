#pragma once
#include "../Render/ImGUI/imgui.h"

class Vec2 : public ImVec2 {};

class Vec3 {
public:
	float x, y, z;

	Vec3(float x = 0.f, float y = 0.f, float z = 0.f) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};