#pragma once
#include "../Hook.hpp"
#include "../../../Events/Event.hpp"

class ActorBaseTick : public Hook
{
public:
	typedef void(__thiscall *detour)(uintptr_t a1);
	static inline detour func_original = 0;

	ActorBaseTick() : Hook("Actor Base Tick", "")
	{
		static auto FunctionOffset = 26;
		auto Vtable = Memory::getVFT(getSig("ActorVFT"));
		Memory::hookFunc(Vtable[FunctionOffset], &callback, (void**)&func_original, "Actor Base Tick");
	}
	static inline int lol = 0;
	static void callback(uintptr_t a1)
	{
		nes::event_holder<TickEvent> event;
		EventDispatcher.trigger(event);
		func_original(a1);
	}
};