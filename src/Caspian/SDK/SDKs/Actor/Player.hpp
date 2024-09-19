#pragma once
#include "mob.hpp"

class Player : public mob {
public:
	BUILD_ACCESS(this, std::string, name, 0x1D30);

	std::string getXuid() {
		return Memory::Call_vft<245, std::string>(this);
	}
};