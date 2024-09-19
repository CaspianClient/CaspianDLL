#pragma once

#include "SDKs/ClientInstance.hpp"
#include "SDKs/Render/ScreenView/ScreenView.hpp"

class SDK {
public:
	static ClientInstance* CI;
	static std::string TopLayer;
	static Packet* createPacket(int packet);
};