#pragma once
#include "Actor/LocalPlayer.hpp"
#include "Block/BlockSource.hpp"
#include "../../Memory/Memory.hpp"

class ClientInstance {
public:

	BUILD_ACCESS(this, LoopbackPacketSender*, packetSender, 0xF0);

	LocalPlayer* getLocalPlayer() {
		return Memory::Call_vft<29, LocalPlayer*>(this);
	}

	BlockSource* getBlockSource() {
		return Memory::Call_vft<28, BlockSource*>(this);
	}

	void GrabMouse() {
		Memory::Call_vft<330, void>(this);
	}

	void ReleaseMouse() {
		Memory::Call_vft<331, void>(this);
	}
};