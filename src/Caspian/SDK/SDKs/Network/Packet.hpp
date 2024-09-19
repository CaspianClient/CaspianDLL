#pragma once
#include "../../../Memory/Memory.hpp"

class Packet {
public:
	int32_t priority = 2;
	int32_t reliability = 1;
	uint8_t subClientId = 0;
	bool isHandled = false;
	void* unknown = nullptr;
	void*** handler = nullptr;
	int32_t compressibility = 0;
};