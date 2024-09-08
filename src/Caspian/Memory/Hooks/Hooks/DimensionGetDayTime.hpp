#pragma once
#include "../Hook.hpp"
#include "../../../Events/Event.hpp"

class DimensionGetDayTime : public Hook
{
public:
    typedef float(__thiscall *detour)(void *a1, float * a2);
    static inline detour func_original = 0;

    DimensionGetDayTime() : Hook("Dimension GetDayTime", "DimensionGetDayTime")
    {
        this->HookFunc(callback, (void **)&func_original);
    }

    static float callback(void *a1, float * a2)
    {
        nes::event_holder<GetTimeOfDayEvent> event;
        event->time = func_original(a1, a2);
        EventDispatcher.trigger(event);
        return event->time;
    }
};