#pragma once
#include "../Hook.hpp"
#include "../../../Events/Event.hpp"

class GameModeAttack : public Hook
{
public:
	typedef void(__thiscall *detour)(void *a1, void* a2);
	static inline detour func_original = 0;

	GameModeAttack() : Hook("GameModeAttack", "GameModeAttack")
	{
		this->HookFunc(callback, (void **)&func_original);
	}

	static void callback(void *a1, void* a2)
	{
		func_original(a1, a2);
		nes::event_holder<GameModeAttackEvent> event;
		event->entity = a2;
        EventDispatcher.trigger(event);
	}
};