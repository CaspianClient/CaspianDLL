#pragma once
#include <iostream>

class Init {
public:
	static void Initialize();
	static void EjectClient();
	static bool isDisabled();
private:
	static bool disabled;
};