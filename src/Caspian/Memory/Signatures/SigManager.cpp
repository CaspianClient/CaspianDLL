#include "SigManager.hpp"

std::map<std::string, std::string> SigManager::Sigs = {};
std::map<std::string, uintptr_t> SigManager::ScannedSigs = {};

void SigManager::AddSig(std::string name, std::string sig) {
	Sigs[name] = sig;
}

uintptr_t SigManager::GetSig(std::string name) {
	return ScannedSigs[name];
}

void SigManager::IntializeSigs() {

	AddSig("Mouse::feed", "? ? ? ? ? ? ? ? 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F B7 BC 24 ? ? ? ? 48 8B D9");
	AddSig("Keyboard::feed", "48 ?? ?? ?? 0F B6 ?? 4C ?? ?? ?? ?? ?? ?? 89 ?? ?? ?? 88");
	AddSig("GetFOV", "48 8B ? 48 89 ? ? 48 89 ? ? 57 48 81 EC ? ? ? ? 0F 29 ? ? 0F 29 ? ? 44 0F ? ? ? 44 0F ? ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? 41 0F");
	AddSig("SetupAndRender", "48 8B C4 48 89 58 18 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 B8 FD FF FF 48 81 EC 10 03 00 00 0F 29 70 B8 0F 29 78 A8 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 E0 01 00 00 4C");
	AddSig("MoveInputComponent", "48 89 5C 24 08 57 48 83 EC 30 48 8B DA BA 2E CD 8B 46");
	AddSig("ActorRotationComponent", "48 89 5C 24 08 57 48 83 EC 30 48 8B DA BA CE 21 1E DC");
	AddSig("ActorHeadRotationComponent", "48 89 5C 24 08 57 48 83 EC 30 48 8B DA BA 1C 58 40 E9");
	AddSig("StateVectorComponent", "48 89 5C 24 08 57 48 83 EC 30 48 8B DA BA 91 3C C9 0E"); //0ec93c91
	AddSig("ActorVFT", "48 8D 05 ? ? ? ? 48 89 01 B8 ? ? ? ? 8D 50 FA 44 8D 48 ? 44 8D 40 ? 66 89 44 ? ? E8 ? ? ? ? 48 8B 8B");
	AddSig("GetGamma", "48 83 EC 28 80 B9 38 17 00 00 00 48 8D 54 24 30 48 8B 01 48 8B 40 60 74 38 41 B8 19");
	AddSig("ActorIntersects", "48 83 EC 28 48 8B 81 98 02 00 00 48 85 C0 74 50");
	AddSig("GetPerspective", "48 83 EC 28 48 8B 01 48 8D 54 24 30 41 B8 03 00");
    AddSig("DimensionGetDayTime", "48 89 5C 24 18 57 48 83 EC 30 48 8B F9 0F 29 74 24 20 48 8B 89 C0 00 00 00");
	AddSig("GetAveragePing", "48 81 EC C8 00 00 00 4C 8B D1 48 8D 4C 24 20 E8 ?? ?? ?? ?? 48 8B D0 45 33 C0 45 33 C9 49 8B CA E8 ?? ?? ?? ?? 4C 8B C0");
	AddSig("OnAppSuspend", "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 55 48 8D 6C 24 A9 48 81 EC 90 00 00 00 48 8B F1 E8 ?? ?? ?? ?? 48 8B D8");
	for (auto it = Sigs.begin(); it != Sigs.end(); ++it) {
		ScannedSigs[it->first] = Memory::ScanSig(it->second);
	}
}