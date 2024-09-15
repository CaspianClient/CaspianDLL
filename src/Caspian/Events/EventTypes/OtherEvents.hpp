#pragma once

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