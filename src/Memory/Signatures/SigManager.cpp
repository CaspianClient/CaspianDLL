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

	AddSig("Mouse::feed", "48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F B7 BC 24 ? ? ? ? 48 8B D9");
	AddSig("Keyboard::feed", "48 ?? ?? ?? 0F B6 ?? 4C ?? ?? ?? ?? ?? ?? 89 ?? ?? ?? 88");

	for (auto it = Sigs.begin(); it != Sigs.end(); ++it) {
		ScannedSigs[it->first] = Memory::ScanSig(it->second);
	}
}