#pragma once
#include "../ClientInstance.hpp"
#include "../../../Memory/Memory.hpp"

class MCUIRC {
public:
	BUILD_ACCESS(this, ClientInstance*, clientInstance, 0x8);
};