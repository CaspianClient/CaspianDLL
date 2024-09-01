#pragma once
#include <iostream>
#include <libhat/Scanner.hpp>
#include <windows.h>
#include <string.h>
#include <assert.h>
#include <MinHook.h>
#include <format>

template <typename Ret, typename Type> Ret& direct_access(Type* type, size_t offset) {
    union {
        size_t raw;
        Type* source;
        Ret* target;
    } u;
    u.source = type;
    u.raw += offset;
    return *u.target;
}

#define AS_FIELD(type, name, fn) __declspec(property(get = fn, put = set##name)) type name

#define BUILD_ACCESS(ptr, type, name, offset)                                                                        \
AS_FIELD(type, name, get##name);                                                                                     \
type get##name() const { return direct_access<type>(ptr, offset); }												     \
void set##name(type v) const { direct_access<type>(ptr, offset) = v; }


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

    template <unsigned int IIdx, typename TRet, typename... TArgs>
    static inline auto Call_vft(void* thisptr, TArgs... argList) -> TRet {
        using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
        return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, argList...);
    }

    static uintptr_t** getVFT(uintptr_t vft) {
        int VFToffset = *reinterpret_cast<int *>(vft + 3);
        uintptr_t **Table = reinterpret_cast<uintptr_t **>(vft + VFToffset + 7);
        return Table;
    }
};