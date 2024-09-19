#include "SDK.hpp"

ClientInstance* SDK::CI = nullptr;
std::string SDK::TopLayer = "";

Packet* SDK::createPacket(int packet) {
		using efunc = std::shared_ptr<Packet>(__thiscall*)(int);
		auto func = reinterpret_cast<efunc>(Memory::ScanSig("40 53 48 83 EC 30 45 33 C0 48 8B D9 FF CA 81 FA 34 01 00 00"));
		return func(packet).get();
	}