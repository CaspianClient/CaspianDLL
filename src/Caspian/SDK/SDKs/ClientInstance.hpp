#pragma once
#include "Actor/LocalPlayer.hpp"
#include "../../Memory/Memory.hpp"

class ClientInstance {
public:
	LocalPlayer* getLocalPlayer() {
		return Memory::Call_vft<29, LocalPlayer*>(this);
	}

	void GrabMouse() {
		Memory::Call_vft<330, void>(this);
	}

	void ReleaseMouse() {
		Memory::Call_vft<331, void>(this);
	}
};