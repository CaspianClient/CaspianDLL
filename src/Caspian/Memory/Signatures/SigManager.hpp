#pragma once
#include <iostream>
#include "../Memory.hpp"
#include <map>

#define getSig(name) SigManager::GetSig(name)

class SigManager {
private:
	static std::map<std::string, std::string> Sigs;
	static std::map<std::string, uintptr_t> ScannedSigs;
	static void AddSig(std::string name, std::string sig);
public:
	static uintptr_t GetSig(std::string name);
	static void IntializeSigs();
};