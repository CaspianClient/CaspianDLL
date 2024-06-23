#pragma once
#include <iostream>

class Init {
public:
	static void Initialize();
	static bool isDisabled();
private:
	static bool disabled;
};