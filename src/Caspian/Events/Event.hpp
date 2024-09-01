#pragma once
#include <nes/event_dispatcher.hpp>
#include "EventTypes/MouseEvent.hpp"
#include "EventTypes/KeyboardEvent.hpp"
#include "EventTypes/OtherEvents.hpp"
#include "EventTypes/RenderEvent.hpp"
#include "EventTypes/TickEvent.hpp"

extern nes::event_dispatcher EventDispatcher;