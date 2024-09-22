#pragma once
#include "../../SDK/SDKs/mce/mce.hpp"

class GetFOVevent {
public:
	float fov;
};

class GetGammaEvent {
public:
	float gamma;
};

class ActorIntersectsEvent {
};

class GetPerspectiveEvent {
public:
	int perspective;
};

class GetTimeOfDayEvent {
public:
    float time;
};

class GetAveragePingEvent {
public:
	int ping;
};

class GameModeAttackEvent {
public:
	void* entity;
};

class RenderMobEffectsUIEvent {
public:
	bool mCancel;
};

class GetOverlayEvent {
public:
	mce::Color color;
};