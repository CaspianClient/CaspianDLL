#pragma once
#include <iostream>
#include <libhat/Scanner.hpp>
#include <windows.h>
#include <string.h>
#include <assert.h>
#include <MinHook.h>
#include <format>

class Memory {
public:
	static auto ScanSig(std::string signature) {
		auto sig = hat::parse_signature(signature);
        assert(sig.has_value());
        auto result = hat::find_pattern(sig.value(), ".text");
        return result.has_result() ? reinterpret_cast<uintptr_t>(result.get()) : NULL;
	}

	static void hookFunc(void* pTarget, void* pDetour, void** ppOriginal, std::string name) {
        if (pTarget == NULL) {
            printf(std::format("{} has invalid address\n", name).c_str());
            return;
        }
        if (MH_CreateHook(pTarget, pDetour, ppOriginal) != MH_OK) {
            printf(std::format("Failed to hook {} function\n", name).c_str());
            return;
        }
        MH_EnableHook(pTarget);
        printf(std::format("Successfully hooked {} function at {}\n", name, pTarget).c_str());
    }
};