#pragma once
#include <MinHook.h>
#include <string.h>
#include "../Memory.hpp"
#include "../Signatures/SigManager.hpp"

#define HookCtor(HookName, HookSig) Hook(HookName, HookSig)

class Hook {
	std::string Name;
	std::string Sig;
public:
	Hook(std::string HookName, std::string HookSig) {
		this->Name = HookName;
		this->Sig = HookSig;
	}
	void HookFunc(void* funcDetour, void** funcOriginal) {
		Memory::hookFunc((void*)getSig(this->Sig), funcDetour, funcOriginal, this->Name);
	}
};