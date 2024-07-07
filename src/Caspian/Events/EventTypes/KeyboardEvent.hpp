#pragma once

class KeyboardEvent {
public:
	int key;
	bool state;
	bool mCancel = false;
};